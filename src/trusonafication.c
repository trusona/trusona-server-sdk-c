/*
 * MIT License
 *
 * Copyright (c) 2016, 2017, 2018 Trusona Inc (www.trusona.com) All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "trusonafication.h"

static const char *ACCEPTED_AT_HIGHER_LEVEL_CODE = "ACCEPTED_AT_HIGHER_LEVEL";
static const char *ACCEPTED_AT_LOWER_LEVEL_CODE  = "ACCEPTED_AT_LOWER_LEVEL";
static const char *ACCEPTED_CODE = "ACCEPTED";
static const char *REJECTED_CODE = "REJECTED";

char *create_trusonafication(struct TrusonaSession trusona_session)
{
  json_t *map = json_object();

  if (trusona_session.input_type == TRUSONA_ID) {
    json_object_set_new(map, "trusona_id", json_string(trusona_session.user_identifier));
  }
  else if (trusona_session.input_type == EMAIL_ADDRESS) {
    json_object_set_new(map, "email", json_string(trusona_session.user_identifier));
  }
  else if (trusona_session.input_type == USER_IDENTIFIER) {
    json_object_set_new(map, "user_identifier", json_string(trusona_session.user_identifier));
    json_object_set_new(map, "user_presence", json_boolean(trusona_session.user_presence));
    json_object_set_new(map, "prompt", json_boolean(trusona_session.prompt));
  }
  else {
    return(NULL);
  }

  json_object_set_new(map, "expires_at", json_string(rfc8601(trusona_session.expires_in_x_seconds)));
  json_object_set_new(map, "desired_level", json_integer(trusona_session.desired_level));
  json_object_set_new(map, "resource", json_string(trusona_session.resource));
  json_object_set_new(map, "action", json_string(trusona_session.action));

  return(json_dumps(map, 0));
}

const enum TRUSONA_SDK_RESULT trusonafy(TrusonaSession trusona_session)
{
  if (!trusona_session.valid) {
    fprintf(stderr, "\nOops! Current settings state is not valid. Bailing!\n");
    syslog(LOG_NOTICE, "%s: Opps! Settings are not valid!", TRUSONA_LIB);
    return(TRUSONA_INIT_ERROR);
  }

  enum TRUSONA_SDK_RESULT rc = TRUSONA_INSUFFICIENT;
  int accepted_level         = INT_MIN;

  char *  status, *json, *body;
  json_t *result_obj;

  body = create_trusonafication(trusona_session);
  json = NULL;

  if (body == NULL) {
    return(TRUSONA_INSUFFICIENT);
  }

  syslog(LOG_NOTICE, "%s: created trusonafication: %s\n", TRUSONA_LIB, body);

  if (do_post_request(trusona_session, trusona_session.trusonafications_uri, body, &json) == INVALID_REQ) {
    return(rc = TRUSONA_SERVICE_ERROR);
  }

  const char *trusonafication_id = json_str_value(&json, "id");
  char *      uri = calloc(1, sizeof(char) * MAX_STR);

  if (uri != NULL && trusonafication_id != NULL) {
    append_str(&uri, trusona_session.trusonafications_uri);
    append_str(&uri, "/");
    append_str(&uri, trusonafication_id);
  }

  unsigned int cnt = 0;
  json = NULL;

  while (cnt < TRUSONA_MAX_WAIT)
  {
    if (do_get_request(trusona_session, uri, &json) == INVALID_REQ) {
      rc = TRUSONA_INSUFFICIENT;
      break;
    }
    else {
      cnt += TRUSONA_SLEEP;
    }

    if (json) {
      status     = (char *)json_str_value(&json, "status");
      result_obj = (json_t *)get_object(&json, "result");

      if (result_obj != NULL) {
        char *result_json;

        result_json    = json_dumps(result_obj, 0);
        accepted_level = (int)json_int_value(&result_json, "accepted_level");

        free(result_json);

        result_obj  = NULL;
        result_json = NULL;
      }
      else {
        accepted_level = INT_MIN;
      }
    }

    if (status && (strcmp(status, ACCEPTED_CODE) == 0 || strcmp(status, ACCEPTED_AT_HIGHER_LEVEL_CODE) == 0)) {
      rc = TRUSONA_SUCCESS;
      break;
    }
    else if (status && strcmp(status, ACCEPTED_AT_LOWER_LEVEL_CODE) == 0) {
      if (accepted_level >= trusona_session.desired_level) {
        rc = TRUSONA_SUCCESS;
        break;
      }
      else {
        syslog(LOG_NOTICE, "%s: Opps! Response's accepted_level of %d does not meet or exceed desired_level of %d.",
               TRUSONA_LIB, accepted_level, trusona_session.desired_level);

        rc = TRUSONA_INSUFFICIENT;
        break;
      }
    }
    else if (status && strcmp(status, REJECTED_CODE) == 0) {
      rc = TRUSONA_FAILURE;
      break;
    }
    else {
      rc     = TRUSONA_INSUFFICIENT;
      status = NULL;
      json   = NULL;

      sleep(TRUSONA_SLEEP);
    }
  }

  json = NULL;

  return(rc);
}
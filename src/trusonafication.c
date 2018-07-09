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

static struct SettingsStruct settings;

static const char *ACCEPTED_AT_HIGHER_LEVEL_CODE = "ACCEPTED_AT_HIGHER_LEVEL";
static const char *ACCEPTED_AT_LOWER_LEVEL_CODE  = "ACCEPTED_AT_LOWER_LEVEL";
static const char *ACCEPTED_CODE = "ACCEPTED";
static const char *REJECTED_CODE = "REJECTED";

static const char *lib_module_name = "trusona";

const enum TRUSONA_SDK_RESULT trusonafy_by_user_identifier(const char *user_identifier)
{
  return TRUSONA_SUCCESS;
}

const enum TRUSONA_SDK_RESULT trusonafy_by_type(const enum SDK_INPUT_TYPE sdk_input_type, const char *value)
{
  if (!settings.valid) {
    syslog(LOG_NOTICE, "%s: Opps! Settings are not valid!", lib_module_name);
    return(TRUSONA_SERVICE_ERR);
  }

  enum TRUSONA_SDK_RESULT rc = TRUSONA_INSUFFICIENT;
  char *  status, *json, *body, *expires_at;
  int     accepted_level;
  json_t *map;

  map = json_object();

  if (sdk_input_type == TRUSONA_ID) {
    json_object_set_new(map, "trusona_id", json_string(value));
  }
  else if (sdk_input_type == EMAIL_ADDRESS) {
    json_object_set_new(map, "email", json_string(value));
  }
  else {
    return(TRUSONA_SERVICE_ERR);
  }

  expires_at = rfc8601(settings.expires_in_x_seconds);

  json_object_set_new(map, "expires_at", json_string(expires_at));
  json_object_set_new(map, "desired_level", json_integer(settings.desired_level));
  json_object_set_new(map, "resource", json_string(settings.resource));
  json_object_set_new(map, "action", json_string(settings.action));

  body = json_dumps(map, 0);
  json = NULL;

  if (do_post_request(settings, settings.trusonafications_uri, body, &json) == INVALID_REQ) {
    return(TRUSONA_SERVICE_ERR);
  }

  const char *trusonafication_id = json_str_value(&json, "id");
  char *      uri = calloc(1, sizeof(char) * MAX_STR);

  if (uri != NULL && trusonafication_id != NULL) {
    append_str(&uri, settings.trusonafications_uri);
    append_str(&uri, "/");
    append_str(&uri, trusonafication_id);
  }

  unsigned int cnt = 0;
  json = NULL;

  while (cnt < TRUSONA_MAX_WAIT)
  {
    do_get_request(settings, uri, &json);
    cnt += TRUSONA_SLEEP;

    if (json) {
      accepted_level = (int)json_int_value(&json, "accepted_level");
      status         = (char *)json_str_value(&json, "status");
    }

    if (status && (strcmp(status, ACCEPTED_CODE) == 0 || strcmp(status, ACCEPTED_AT_HIGHER_LEVEL_CODE) == 0)) {
      rc = TRUSONA_SUCCESS;
      break;
    }
    else if (status && strcmp(status, ACCEPTED_AT_LOWER_LEVEL_CODE) == 0) {
      if (accepted_level >= settings.desired_level) {
        rc = TRUSONA_SUCCESS;
        break;
      }
      else {
        syslog(LOG_NOTICE, "%s: Opps! Response's accepted_level of %d does not meet or exceed desired_level of %d",
               lib_module_name, accepted_level, settings.desired_level);

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

  expires_at = NULL;
  json       = NULL;

  return(rc);
}
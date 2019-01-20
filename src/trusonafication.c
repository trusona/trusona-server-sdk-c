// Apache License. Version 2.0, January 2004
//
// Copyright (c) 2016, 2017, 2018 Trusona Inc (https://trusona.com) All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

  syslog(LOG_NOTICE, "%s: JSON sent to create trusonafication: %s\n", TRUSONA_LIB, body);

  if (post_request(trusona_session, trusona_session.trusonafications_uri, body, &json) == INVALID_REQ) {
    syslog(LOG_NOTICE, "%s: Failed to create trusonafication.", TRUSONA_LIB);
    return(rc = TRUSONA_SERVICE_ERROR);
  }

  const char *trusonafication_id = json_str_value(&json, "id");
  status = (char *)json_str_value(&json, "status");

  if (!trusonafication_id || strcmp(status, "IN_PROGRESS") != 0) {
    syslog(LOG_NOTICE, "%s: Failed to successfully create a trusonafication: %s", TRUSONA_LIB, json);
    return(rc = TRUSONA_SERVICE_ERROR);
  }
  else {
    syslog(LOG_NOTICE, "%s: Successfully create a trusonafication: %s", TRUSONA_LIB, json);
  }

  char *uri = calloc(1, sizeof(char) * MAX_STR);

  if (uri != NULL && trusonafication_id != NULL) {
    append_str(&uri, trusona_session.trusonafications_uri);
    append_str(&uri, "/");
    append_str(&uri, trusonafication_id);
  }

  unsigned int cnt = 0;
  json = NULL;

  while (cnt < TRUSONA_MAX_WAIT)
  {
    if (get_request(trusona_session, uri, &json) == INVALID_REQ) {
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
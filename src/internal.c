/*
 * MIT License
 *
 * Copyright (c) 2016, 2017 Trusona Inc (www.trusona.com) All Rights Reserved
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

#include "internal.h"

static struct SettingsStruct settings;

static const char *ACCEPTED_AT_HIGHER_LEVEL_CODE = "ACCEPTED_AT_HIGHER_LEVEL";
static const char *ACCEPTED_AT_LOWER_LEVEL_CODE = "ACCEPTED_AT_LOWER_LEVEL";
static const char *ACCEPTED_CODE = "ACCEPTED";
static const char *REJECTED_CODE = "REJECTED";

static const char *lib_module_name = "trusona";


char *concat_str(const char *arr1, const char *arr2) {
  char *val = calloc(1, sizeof(char) * MAX_STR);

  if(val != NULL) {
    append_str(&val, arr1);
    append_str(&val, arr2);

    int c = strnlen(arr1, MAX_STR) + strnlen(arr2, MAX_STR);
    val[c] = '\0';
  }

  return val;
}


void append_str(char **buffer, const char *data) {
  strncat(*buffer, data, strnlen(data, MAX_STR));
}


char *generate_guid() {
  uuid_t raw_guid;
  char* guid;

  guid = calloc(1, sizeof(char) * 36);

  uuid_generate_random(raw_guid);
  uuid_unparse_upper(raw_guid, guid);

  return guid;
}

char *rfc8601(int from_now_seconds) {
  time_t rawtime;
  struct tm * timeinfo;

  // some realignment... max future time is 5 minutes for our needs
  if(from_now_seconds > 300 || from_now_seconds <= 0) {
    from_now_seconds = TRUSONA_MAX_WAIT;
  }

  char *val = calloc(1, sizeof(char) * MAX_STR);

  long secs = from_now_seconds + time (&rawtime);
  timeinfo = gmtime(&secs);

  strftime (val, MAX_STR, "%Y-%m-%dT%H:%M:%SZ", timeinfo);

  free(timeinfo);
  timeinfo = NULL;

  return val;
}

char *now_rfc1123() {
  time_t rawtime;
  struct tm * timeinfo;

  char *val = calloc(1, sizeof(char) * MAX_STR);

  time (&rawtime);
  timeinfo = gmtime(&rawtime);

  strftime (val, MAX_STR, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

  return val;
}

SettingsStruct load_settings(const char *json_settings_file) {
  json_error_t *error = NULL;
  json_t *root;

  root = json_load_file(json_settings_file, 0, error);
  settings.valid = false;

  if(root) {
    settings.expires_in_x_seconds = (int)get_int_value(root, "expires_in_x_seconds");
    settings.access_token = (char *)get_str_value(root, "access_token");
    settings.desired_level = (int)get_int_value(root, "desired_level");
    settings.resource = (char *)get_str_value(root, "resource");
    settings.api_host = (char *)get_str_value(root, "api_host");
    settings.mac_key = (char *)get_str_value(root, "mac_key");
    settings.action = (char *)get_str_value(root, "action");

    settings.request_id = generate_guid();

    // hard-coding values that will never change
    settings.trusonafications_uri = "/api/v2/trusonafications";
    settings.token_type = "TRUSONA";

    settings.valid = true;

    syslog(LOG_NOTICE, "%s: Yea! Settings loaded from '%s'", lib_module_name, json_settings_file);
  }

  return settings;
}

const enum TRUSONA_SDK_RESULT trusonafy_by_type(const enum API_INPUT_TYPE api_input_type, const char *value) {
  if(!settings.valid) {
    syslog(LOG_NOTICE, "%s: Opps! Settings are not valid!", lib_module_name);
    return TRUSONA_SERVICE_ERR;
  }

  enum TRUSONA_SDK_RESULT rc = TRUSONA_INSUFFICIENT;
  char *status, *json, *body, *expires_at;
  int accepted_level;
  json_t *map;

  map = json_object();

  if(api_input_type == TRUSONA_ID) {
    json_object_set_new(map, "trusona_id", json_string(value));
  }
  else if (api_input_type == EMAIL_ADDRESS) {
    json_object_set_new(map, "email", json_string(value));
  }
  else {
    return TRUSONA_SERVICE_ERR;
  }

  expires_at = rfc8601(settings.expires_in_x_seconds);

  json_object_set_new(map, "expires_at", json_string(expires_at));
  json_object_set_new(map, "desired_level", json_integer(settings.desired_level));
  json_object_set_new(map, "resource", json_string(settings.resource));
  json_object_set_new(map, "action", json_string(settings.action));

  body = json_dumps(map, 0);
  json = NULL;

  if(do_post_request(settings, settings.trusonafications_uri, body, &json) == INVALID_REQ) {
    return TRUSONA_SERVICE_ERR;
  }

  const char *trusonafication_id = json_str_value(&json, "id");
  char *uri = calloc(1, sizeof(char) * MAX_STR);

  if(uri != NULL && trusonafication_id != NULL) {
    append_str(&uri, settings.trusonafications_uri);
    append_str(&uri, "/");
    append_str(&uri, trusonafication_id);
  }

  unsigned int cnt = 0;
  json = NULL;

  while(cnt < TRUSONA_MAX_WAIT) {
    do_get_request(settings, uri, &json);
    cnt += TRUSONA_SLEEP;

    if(json) {
      accepted_level = (int)json_int_value(&json, "accepted_level");
      status = (char *)json_str_value(&json, "status");
    }

    if(status && (strcmp(status, ACCEPTED_CODE) == 0 || strcmp(status, ACCEPTED_AT_HIGHER_LEVEL_CODE) == 0)) {
      rc = TRUSONA_SUCCESS;
      break;
    }
    else if (status && strcmp(status, ACCEPTED_AT_LOWER_LEVEL_CODE) == 0) {
      if(accepted_level >= settings.desired_level) {
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
    else if(status && strcmp(status, REJECTED_CODE) == 0) {
      rc = TRUSONA_FAILURE;
      break;
    }
    else {
      rc = TRUSONA_INSUFFICIENT;
      status = NULL;
      json = NULL;

      sleep(TRUSONA_SLEEP);
    }
  }

  free(expires_at);
  free(json);

  expires_at = NULL;
  json = NULL;

  return rc;
}
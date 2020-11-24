// Apache License. Version 2.0, January 2004
//
// Copyright (c) 2016-2020 Trusona Inc (https://trusona.com) All Rights Reserved
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

#include "internal.h"

char *generate_guid()
{
  uuid_t raw_guid;
  char * guid;

  guid = calloc(1, sizeof(char) * 36);

  uuid_generate_random(raw_guid);
  uuid_unparse_upper(raw_guid, guid);

  return(guid);
}

char *rfc8601(int from_now_seconds)
{
  time_t     rawtime;
  struct tm *timeinfo;

  // some realignment... max future time is 5 minutes for our needs
  if (from_now_seconds > 300 || from_now_seconds <= 0) {
    from_now_seconds = TRUSONA_MAX_WAIT;
  }

  char *val = calloc(1, sizeof(char) * MAX_STR);

  long secs = from_now_seconds + time(&rawtime);

  timeinfo = gmtime(&secs);

  strftime(val, MAX_STR, "%Y-%m-%dT%H:%M:%SZ", timeinfo);

  timeinfo = NULL;

  return(val);
}

char *now_rfc1123()
{
  time_t     rawtime;
  struct tm *timeinfo;

  char *val = calloc(1, sizeof(char) * MAX_STR);

  time(&rawtime);
  timeinfo = gmtime(&rawtime);

  strftime(val, MAX_STR, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);

  timeinfo = NULL;

  return(val);
}

TrusonaSession load_settings(const char *json_settings_file)
{
  json_error_t *error = NULL;
  json_t *      root;

  root = json_load_file(json_settings_file, 0, error);

  struct TrusonaSession trusona_settings;

  trusona_settings.valid = false;

  if (root) {
    trusona_settings.expires_in_x_seconds = (int)get_int_value(root, "expires_in_x_seconds");

    trusona_settings.access_token  = (char *)get_str_value(root, "access_token");
    trusona_settings.desired_level = (int)get_int_value(root, "desired_level");
    trusona_settings.resource      = (char *)get_str_value(root, "resource");
    trusona_settings.api_host      = (char *)get_str_value(root, "api_host");
    trusona_settings.mac_key       = (char *)get_str_value(root, "mac_key");
    trusona_settings.action        = (char *)get_str_value(root, "action");

    trusona_settings.request_id = generate_guid();

    trusona_settings.trusonafications_uri = "/api/v2/trusonafications";
    trusona_settings.token_type           = "TRUSONA";

    trusona_settings.valid = true;

    syslog(LOG_NOTICE, "%s: Yea! Settings loaded from '%s'", TRUSONA_LIB, json_settings_file);
  }

  error = NULL;
  root  = NULL;

  return(trusona_settings);
}
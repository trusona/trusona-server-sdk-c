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

#include "trusona.h"

static struct TrusonaSession trusona_session;

static const char *default_settings = "/usr/local/etc/trusona/settings.json";       // the default location

const bool init(const char *json_settings_file)
{
  syslog(LOG_NOTICE, "%s: Hold on to your C pants - here we go!", TRUSONA_LIB);

  if (json_settings_file == NULL) {
    syslog(LOG_ERR, "%s: Using settings' default location of '%s'", TRUSONA_LIB, default_settings);
    trusona_session = load_settings((char *)default_settings);
  }
  else {
    trusona_session = load_settings(json_settings_file);
  }

  if (!trusona_session.valid) {
    syslog(LOG_ERR, "%s: Failed to load Trusona settings from '%s' or '%s'", TRUSONA_LIB, json_settings_file, default_settings);
    return(FALSE);
  }
  else if (trusona_session.desired_level < MIN_LOA_LEVEL || trusona_session.desired_level > MAX_LOA_LEVEL) {
    syslog(LOG_ERR, "%s: Oops! desired_level setting is invalid with a value of '%d'", TRUSONA_LIB, trusona_session.desired_level);
    return(FALSE);
  }

  syslog(LOG_NOTICE, "%s: Minimum LOA (also used in the API as 'desired_level') is %d", TRUSONA_LIB, trusona_session.desired_level);
  syslog(LOG_NOTICE, "%s: We appear to be correctly configured.", TRUSONA_LIB);

  fprintf(stderr, "Respond to Trusona via your mobile device. You have %d seconds...", trusona_session.expires_in_x_seconds);

  return(TRUE);
}

const enum TRUSONA_INPUT get_input_type(const char *value)
{
  if (value != NULL && strlen(value) > 0) {
    if (strchr(value, '@') != NULL) {
      return(EMAIL_ADDRESS);
    }

    if (strlen(value) == 9) {
      bool digit = true;
      int  i;

      for (i = 0; digit && i < 9; i++) {
        digit = value[i] >= 48 && value[i] <= 57;
      }

      return(digit ? TRUSONA_ID : UNKNOWN);
    }
  }

  return(UNKNOWN);
}

const enum TRUSONA_SDK_RESULT trusonafy_v2(const char *json_settings_file, const char *user_identifier)
{
  return(trusonafy_v2_ext(json_settings_file, user_identifier, true, true));
}

const enum TRUSONA_SDK_RESULT trusonafy_v2_ext(const char *json_settings_file, const char *user_identifier, const bool prompt, const bool presence)
{
  if (init(json_settings_file)) {
    syslog(LOG_NOTICE, "%s: %s: Attempting trusonafication for '%s'", TRUSONA_LIB, trusona_session.request_id, user_identifier);
  }
  else {
    return(TRUSONA_INIT_ERROR);
  }

  trusona_session.user_identifier = (char *)user_identifier;
  trusona_session.input_type      = USER_IDENTIFIER;
  trusona_session.user_presence   = presence;
  trusona_session.prompt          = prompt;

  const enum TRUSONA_SDK_RESULT rc = trusonafy(trusona_session);

  if (rc == TRUSONA_SUCCESS) {
    syslog(LOG_NOTICE, "%s: %s: Yeah! Trusonafication succeeded for '%s'", TRUSONA_LIB, trusona_session.request_id, user_identifier);
    fprintf(stderr, "\nTrusonafication succeeded!\n");
  }
  else{
    syslog(LOG_ERR, "%s: %s: Oops! Trusonafication failed for '%s'", TRUSONA_LIB, trusona_session.request_id, user_identifier);
    fprintf(stderr, "\nTrusonafication failed!\n");
  }

  return(rc);
}

const enum TRUSONA_SDK_RESULT trusonafy_v1(const char *json_settings, const char *value)
{
  if (init(json_settings)) {
    syslog(LOG_NOTICE, "%s: %s: Attempting trusonafication for '%s'", TRUSONA_LIB, trusona_session.request_id, value);
  }
  else {
    return(TRUSONA_INIT_ERROR);
  }

  trusona_session.user_identifier = (char *)value;
  trusona_session.input_type      = get_input_type(value);

  const enum TRUSONA_SDK_RESULT rc = trusonafy(trusona_session);

  if (rc == TRUSONA_SUCCESS) {
    syslog(LOG_NOTICE, "%s: %s: Yeah! Trusonafication succeeded for '%s'", TRUSONA_LIB, trusona_session.request_id, value);
    fprintf(stderr, "\nTrusonafication succeeded!\n");
  }
  else {
    syslog(LOG_ERR, "%s: %s: Oops! Trusonafication failed for '%s'", TRUSONA_LIB, trusona_session.request_id, value);
    fprintf(stderr, "\nTrusonafication failed!\n");
  }

  return(rc);
}
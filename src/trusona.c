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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "trusona.h"

static struct TrusonaSession trusona_session;

static const char *lib_module_name  = "trusona";
static const char *default_settings = "/usr/local/etc/trusona/settings.json";       // the default location

const int init(const char *json_settings_file)
{
  syslog(LOG_NOTICE, "%s: Hold on to your C pants - here we go!", lib_module_name);

  if (json_settings_file == NULL) {
    syslog(LOG_ERR, "%s: Using settings' default location of '%s'", lib_module_name, default_settings);
    trusona_session = load_settings((char *)default_settings);
  }
  else {
    trusona_session = load_settings(json_settings_file);
  }

  if (!trusona_session.valid) {
    syslog(LOG_ERR, "%s: Failed to load Trusona settings from '%s' or '%s'", lib_module_name, json_settings_file, default_settings);
    return(-1);
  }
  else if (trusona_session.desired_level < MIN_LOA_LEVEL || trusona_session.desired_level > MAX_LOA_LEVEL) {
    syslog(LOG_ERR, "%s: Oops! desired_level setting is invalid with a value of '%d'", lib_module_name, trusona_session.desired_level);
    return(-1);
  }

  syslog(LOG_NOTICE, "%s: Minimum LOA (also used in the API as 'desired_level') is %d", lib_module_name, trusona_session.desired_level);
  syslog(LOG_NOTICE, "%s: We appear to be correctly configured.", lib_module_name);

  fprintf(stderr, "Respond to Trusona via your mobile device. You have %d seconds...", trusona_session.expires_in_x_seconds);

  return(0);
}

const enum INPUT_TYPE get_input_type(const char *value)
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

      return(digit ? TRUSONA_ID : INVALID_TYPE);
    }
  }

  return(INVALID_TYPE);
}

const enum TRUSONA_SDK_RESULT trusonafy_v2(const char *json_settings_file, const char *user_identifier)
{
  return(trusonafy_v2_ext(json_settings_file, user_identifier, true, true));
}

const enum TRUSONA_SDK_RESULT trusonafy_v2_ext(const char *json_settings_file, const char *user_identifier, const bool prompt, const bool presence)
{
  const int state = init(json_settings_file);

  if (state == 0) {
    syslog(LOG_NOTICE, "%s: %s: Attempting trusonafication for '%s'", lib_module_name, trusona_session.request_id, user_identifier);
  }
  else {
    return(TRUSONA_INIT_ERR);
  }

  trusona_session.user_identifier = (char *)user_identifier;
  trusona_session.input_type      = USER_IDENTIFIER;
  trusona_session.user_presence   = presence;
  trusona_session.prompt          = prompt;

  const enum TRUSONA_SDK_RESULT rc = trusonafy(trusona_session);

  if (rc == TRUSONA_SUCCESS) {
    syslog(LOG_NOTICE, "%s: %s: Yeah! Trusonafication succeeded for '%s'", lib_module_name, trusona_session.request_id, user_identifier);
    fprintf(stderr, "\nTrusonafication succeeded!\n");
  }
  else{
    syslog(LOG_ERR, "%s: %s: Oops! Trusonafication failed for '%s'", lib_module_name, trusona_session.request_id, user_identifier);
    fprintf(stderr, "\nTrusonafication failed!\n");
  }

  return(rc);
}

const enum TRUSONA_SDK_RESULT trusonafy_v1(const char *json_settings, const char *value)
{
  const int state = init(json_settings);

  if (state == 0) {
    syslog(LOG_NOTICE, "%s: %s: Attempting trusonafication for '%s'", lib_module_name, trusona_session.request_id, value);
  }
  else {
    return(TRUSONA_INIT_ERR);
  }

  trusona_session.user_identifier = (char *)value;
  trusona_session.input_type      = get_input_type(value);

  const enum TRUSONA_SDK_RESULT rc = trusonafy(trusona_session);

  if (rc == TRUSONA_SUCCESS) {
    syslog(LOG_NOTICE, "%s: %s: Yeah! Trusonafication succeeded for '%s'", lib_module_name, trusona_session.request_id, value);
    fprintf(stderr, "\nTrusonafication succeeded!\n");
  }
  else {
    syslog(LOG_ERR, "%s: %s: Oops! Trusonafication failed for '%s'", lib_module_name, trusona_session.request_id, value);
    fprintf(stderr, "\nTrusonafication failed!\n");
  }

  return(rc);
}
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

#include "trusona.h"

static const char *lib_module_name = "trusona";
static const char *default_settings = "/usr/local/etc/trusona/settings.json"; // the default location

int trusonafy(const char* runtime_settings, const char* value) {
  syslog(LOG_NOTICE, "%s: Hold on to your C pants - here we go!", lib_module_name);
  char *settings = NULL;

  if (runtime_settings == NULL) {
    syslog(LOG_ERR, "%s: Using settings' default location of '%s'", lib_module_name, default_settings);
    settings = (char *)default_settings;
  }

  SettingsStruct global_settings = load_settings(settings);

  if(!global_settings.valid) {
    syslog(LOG_ERR, "%s: Failed to load Trusona settings from '%s'", lib_module_name, settings);
    return TRUSONA_SERVICE_ERR;
  }
  else if(global_settings.desired_level < MIN_LOA_LEVEL || global_settings.desired_level > MAX_LOA_LEVEL) {
    syslog(LOG_ERR, "%s: Oops! desired_level setting is invalid with a value of '%d'", lib_module_name,
           global_settings.desired_level);
    return TRUSONA_SERVICE_ERR;
  }

  syslog(LOG_NOTICE, "%s: Minimum LOA (also used in the API as 'desired_level') is %d", lib_module_name, global_settings.desired_level);
  syslog(LOG_NOTICE, "%s: We appear to be correctly configured.", lib_module_name);

  fprintf(stderr, "Respond to Trusona via your mobile device. You have 99 seconds...");

  syslog(LOG_NOTICE, "%s: %s: Attempting trusonafication for '%s'", lib_module_name, global_settings.request_id, value);
  const int rc = trusonafy_by_type(get_api_input_type(value), value);

  if(rc == TRUSONA_SUCCESS ) {
    syslog(LOG_NOTICE, "%s: %s: Yeah! Trusonafication succeeded for '%s'", lib_module_name, global_settings.request_id, value);
    fprintf(stderr, "\nTrusonafication succeeded!\n");
  }
  else {
    syslog(LOG_ERR, "%s: %s: Oops! Trusonafication failed for '%s'", lib_module_name, global_settings.request_id, value);
    fprintf(stderr, "\nTrusonafication failed!\n");
  }

  return rc;
}

const enum API_INPUT_TYPE get_api_input_type(const char* value) {
  if(value != NULL && strlen(value) > 0) {
    if(strchr(value, '@') != NULL) {
      return EMAIL_ADDRESS;
    }

    if(strlen(value) == 9) {
      bool digit = true;
      int i;

      for(i = 0; digit && i < 9; i++) {
        digit = value[i] >= 48 && value[i] <= 57;
      }

      return digit ? TRUSONA_ID : INVALID_TYPE;
    }
  }

  return INVALID_TYPE;
}
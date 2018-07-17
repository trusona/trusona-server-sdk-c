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

#include "internal.h"

static const char *lib_module_name = "trusona";

char *concat_str(const char *arr1, const char *arr2)
{
  char *val = calloc(1, sizeof(char) * MAX_STR);

  if (val != NULL) {
    append_str(&val, arr1);
    append_str(&val, arr2);

    int c = strnlen(arr1, MAX_STR) + strnlen(arr2, MAX_STR);
    val[c] = '\0';
  }

  return(val);
}

void append_str(char **buffer, const char *data)
{
  strncat(*buffer, data, strnlen(data, MAX_STR));
}

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

    // hard-coding values that will never change
    trusona_settings.trusonafications_uri = "/api/v2/trusonafications";
    trusona_settings.token_type           = "TRUSONA";

    trusona_settings.valid = true;

    syslog(LOG_NOTICE, "%s: Yea! Settings loaded from '%s'", lib_module_name, json_settings_file);
  }

  return(trusona_settings);
}
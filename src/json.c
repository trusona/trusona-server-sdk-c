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

#include "json.h"

const json_t *get_object(char **json, const char *key)
{
  json_error_t error;
  json_t *     root   = json_loads(*json, 0, &error);
  json_t *     result = json_object_get(root, key);

  root = NULL;

  return(result);
}

const char *json_str_value(char **json, const char *data)
{
  json_error_t error;
  json_t *     root = json_loads(*json, 0, &error);

  if (!root) {
    syslog(LOG_NOTICE, "%s: Failed to validate JSON %s", TRUSONA_LIB, *json);
    return(NULL);
  }

  const char *key = NULL;
  char *      val = NULL;
  json_t *    value;

  json_object_foreach(root, key, value)
  {
    if (!val && strcmp(key, data) == 0) {
      val = (char *)json_string_value(value);
    }
  }

  key   = NULL;
  value = NULL;

  return(val);
}

const int json_int_value(char **json, const char *data)
{
  json_error_t error;
  json_t *     root = json_loads(*json, 0, &error);

  if (!root) {
    syslog(LOG_NOTICE, "%s: Failed to validate JSON %s", TRUSONA_LIB, *json);
    return(INT_MIN);
  }

  const char *key;
  json_t *    value;

  json_object_foreach(root, key, value)
  {
    if (strcmp(key, data) == 0) {
      root = NULL;
      key  = NULL;

      return((int)json_integer_value(value));
    }
  }

  root  = NULL;
  key   = NULL;
  value = NULL;

  return(INT_MIN);
}

const int get_int_value(json_t *root, const char *data)
{
  const char *key;
  json_t *    value;

  json_object_foreach(root, key, value)
  {
    if (strcmp(key, data) == 0) {
      key = NULL;
      return((int)json_integer_value(value));
    }
  }

  key = NULL;

  return(INT_MIN);
}

const char *get_str_value(json_t *root, const char *data)
{
  const char *key;
  char *      val = NULL;
  json_t *    value;

  json_object_foreach(root, key, value)
  {
    if (!val && strcmp(key, data) == 0) {
      val = (char *)json_string_value(value);
    }
  }

  key   = NULL;
  value = NULL;

  return(val);
}
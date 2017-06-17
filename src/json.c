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

#include "json.h"

static const char *lib_module_name = "trusona";

const char  *json_str_value(char **json, const char *data) {
  json_error_t error;
  json_t *root = json_loads(*json, 0, &error);

  if(!root) {
    syslog(LOG_NOTICE, "%s: Failed to validate JSON %s", lib_module_name, *json);
    return NULL;
  }

  const char *key;
  char *val = NULL;
  json_t *value;

  json_object_foreach(root, key, value) {
    if(!val && strcmp(key, data) == 0) {
      val = (char*)json_string_value(value);
    }
  }

  return val;
}

const int json_int_value(char **json, const char *data) {
  json_error_t error;
  json_t *root = json_loads(*json, 0, &error);

  if(!root) {
    syslog(LOG_NOTICE, "%s: Failed to validate JSON %s", lib_module_name, *json);
    return INT_MIN;
  }

  const char *key;
  json_t *value;

  json_object_foreach(root, key, value) {
    if(strcmp(key, data) == 0) {
      return (int)json_integer_value(value);
    }
  }

  return INT_MIN;
}

const int get_int_value(json_t  *root, const char  *data) {
  const char *key;
  json_t *value;

  json_object_foreach(root, key, value) {
    if(strcmp(key, data) == 0) {
      return (int)json_integer_value(value);
    }
  }

  return INT_MIN;
}

const char  *get_str_value(json_t  *root, const char  *data) {
  const char *key;
  char *val = NULL;
  json_t *value;

  json_object_foreach(root, key, value) {
    if(!val && strcmp(key, data) == 0) {
      val = (char*)json_string_value(value);
    }
  }

  return val;
}
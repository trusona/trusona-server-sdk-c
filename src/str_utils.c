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

#include "str_utils.h"

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

char *trim(const char *str)
{
  if (str == NULL) {
    return(NULL);
  }

  char *ptr   = calloc(1, sizeof(char) * MAX_STR);
  int   last  = strnlen(str, MAX_STR);
  int   first = -1;
  int   idx   = 0;

  while (isspace(str[++first]))
  {
    // do nothing
  }

  while (isspace(str[--last]))
  {
    // do nothing
  }

  if (first != last) {
    for (; first <= last;) {
      ptr[idx++] = str[first++];
    }
  }

  ptr[idx] = '\0';
  return(ptr);
}
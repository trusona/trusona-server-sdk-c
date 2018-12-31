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

#include <sys/stat.h>
#include "utils.h"

const uid_t owned_by(const char *file)
{
  uid_t uid = UINT_MAX;

  if (access(file, F_OK) == 0) {
    struct stat file_stats;

    if (lstat(file, &file_stats) == 0) {
      if ((file_stats.st_mode & S_IFMT) == S_IFREG) {
        uid = file_stats.st_uid;
      }
      else {
        uid = UINT_MAX - 2;
      }
    }
    else {
      uid = UINT_MAX - 3;
    }
  }
  else {
    uid = UINT_MAX - 4;
  }

  return(uid);
}

const int file_perms(const char *file)
{
  int result = -1;

  if (access(file, F_OK) == 0) {
    struct stat file_stats;
    result = -2;

    if (lstat(file, &file_stats) == 0) {
      if ((file_stats.st_mode & S_IFMT) == S_IFREG) {
        result = 0;

        if (S_IRUSR & file_stats.st_mode) {
          result += 400;
        }
        if (S_IWUSR & file_stats.st_mode) {
          result += 200;
        }
        if (S_IXUSR & file_stats.st_mode) {
          result += 100;
        }
        if (S_IRGRP & file_stats.st_mode) {
          result += 40;
        }
        if (S_IWGRP & file_stats.st_mode) {
          result += 20;
        }
        if (S_IXGRP & file_stats.st_mode) {
          result += 10;
        }
        if (S_IROTH & file_stats.st_mode) {
          result += 4;
        }
        if (S_IWOTH & file_stats.st_mode) {
          result += 2;
        }
        if (S_IXOTH & file_stats.st_mode) {
          result += 1;
        }
        if (S_ISUID & file_stats.st_mode) {
          result += 4000;
        }
        if (S_ISGID & file_stats.st_mode) {
          result += 2000;
        }
        if (S_ISVTX & file_stats.st_mode) {
          result += 1000;
        }
      }
    }
  }

  return(result);
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
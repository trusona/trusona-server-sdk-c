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

#include "pam_internal.h"

const int min(const int x, const int y)
{
  return(x <= y ? x : y);
}

const char *file_contents(const char *filename)
{
  FILE *file = fopen(filename, "r");

  if (!file) {
    return(NULL);
  }

  fseek(file, 0, SEEK_END);
  long file_size = min(ftell(file), 128); // max bytes are 128
  fseek(file, 0, SEEK_SET);

  if (file_size <= 0) {
    fclose(file);
    return(NULL);
  }

  char *file_buffer = calloc(1, sizeof(char) * (file_size + 1)); // plus 1 for \0
  fread(file_buffer, 1, file_size, file);
  fclose(file);
  return(trim(file_buffer));
}

const char *configured_user_identifier(const char *home_dir)
{
  const char *user_identifier = NULL;

  if (home_dir != NULL) {
    char *file = concat_str(home_dir, "/.trusona");

    if (access(file, F_OK) != -1) {
      const int permissions = file_perms(file);

      if (permissions == 600 || permissions == 400) {
        user_identifier = file_contents(file);
        syslog(LOG_NOTICE, "%s: Configured user identifier for trusona is '%s'", TRUSONA_LIB, user_identifier);
      }
      else {
        syslog(LOG_WARNING, "%s: File permissions for '%s' are expected to be 0400 or 0600", TRUSONA_LIB, file);
      }
    }
    else {
      syslog(LOG_NOTICE, "%s: %s/.trusona does not exist or is not a regular file", TRUSONA_LIB, home_dir);
    }
  }

  return(user_identifier);
}
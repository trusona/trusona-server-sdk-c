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

  const int e_uid = geteuid();
  const int c_uid = getuid();

  if (home_dir == NULL) {
    syslog(LOG_WARNING, "%s: $HOME directory cannot be NULL", TRUSONA_LIB);
    return(NULL);
  }
  else if (e_uid != c_uid) {
    syslog(LOG_WARNING, "%s: Effective (getuid()) UID %d and Current (getuid()) UID %d are not the same", TRUSONA_LIB, e_uid, c_uid);
    return(NULL);
  }

  const char *file  = concat_str(home_dir, "/.trusona");
  const int   perms = file_perms(file);
  const int   owner = owned_by(file);

  if (perms < 0 || owner < 0) {
    syslog(LOG_WARNING, "%s: %s/.trusona does not exist or is not a regular file", TRUSONA_LIB, home_dir);
    return(NULL);
  }
  else if (owner != e_uid) {
    syslog(LOG_WARNING, "%s: %s/.trusona is not owned by the current user with ID %d", TRUSONA_LIB, home_dir, e_uid);
    return(NULL);
  }
  else if (perms == 600 || perms == 400) {
    user_identifier = file_contents(file);
    syslog(LOG_NOTICE, "%s: Configured user identifier for trusona is '%s'", TRUSONA_LIB, user_identifier);
  }
  else {
    syslog(LOG_WARNING, "%s: File permissions for '%s' are expected to be 0400 or 0600", TRUSONA_LIB, file);
  }

  return(user_identifier);
}
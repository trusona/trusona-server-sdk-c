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

#include "internal_pam.h"

const char *file_contents(const char *filename)
{
  FILE *file = fopen(filename, "r");

  if (!file) {
    return(NULL);
  }

  fseek(file, 0, SEEK_END);
  long file_size = fmin(ftell(file), MAX_STR / 2 / 2 / 2); // max 128 bytes
  fseek(file, 0, SEEK_SET);

  if (file_size <= 0) {
    fclose(file);
    return(NULL);
  }

  char *file_buffer = calloc(1, sizeof(char) * file_size);
  fread(file_buffer, 1, file_size, file);
  fclose(file);
  return(trim(file_buffer));
}

const char *configured_user_identifier(const char *home_dir)
{
  if (home_dir == NULL) {
    return(NULL);
  }

  char *file = concat_str(home_dir, "/.trusona");

  syslog(LOG_NOTICE, "%s: %s", TRUSONA_LIB, file);

  if (access(file, F_OK) != -1) {
    struct stat file_stats;

    if (stat(file, &file_stats) >= 0) {
      if (S_ISREG(file_stats.st_mode)) {
        // todo: check file permissions and enforce 100400 or 100600 only
        // also, enforce file ownership in that current $USER must be the owner of the .trusona file
        return(file_contents(file));
      }
    }
  }

  return(NULL);
}
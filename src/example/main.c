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

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "../trusona.h"

const char *json_settings = "/usr/local/etc/trusona/settings.json";

// tilted options: --user <any_user_identifier> --prompt --presence

int main(int argc, char *argv[])
{
  enum TRUSONA_SDK_RESULT result = TRUSONA_INSUFFICIENT;

  char *user_identifier = NULL;
  bool  presence        = FALSE;
  bool  prompt          = FALSE;
  int   idx             = -1;

  static struct option options[] =
  {
    { "prompt",   no_argument,       NULL, 0 },
    { "presence", no_argument,       NULL, 0 },
    { "user",     required_argument,    0, 0 },
  };

  while (getopt_long_only(argc, argv, "", options, &idx) != -1)
  {
    if (strcmp("presence", options[idx].name) == 0) {
      presence = TRUE;
      continue;
    }

    if (strcmp("prompt", options[idx].name) == 0) {
      prompt = TRUE;
      continue;
    }

    if (strcmp("user", options[idx].name) == 0) {
      user_identifier = optarg;
      continue;
    }
  }

  if (user_identifier != NULL) {
    printf("Sending trusonafication to '%s'\n", user_identifier);
    printf("JSON settings will load from %s\n", json_settings);
    result = trusonafy_v2_ext(json_settings, user_identifier, prompt, presence);
  }

  return(result);
}
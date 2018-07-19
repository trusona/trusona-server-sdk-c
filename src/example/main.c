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

#include <getopt.h>

#include "../trusona.h"

const char *json_settings = "/usr/local/etc/trusona/settings.json";

// tilted options: --settings <path_to_json_settings> --user <any_user_identifier> --prompt --presence --tilted

int main(int argc, char *argv[])
{
  if (argc <= 1) {
    char *user     = "--user <any_user_identifier> - required; user to send trusonafication to";
    char *settings = "--settings <path_to_json_settings> - optional; otherwise will use the default";
    char *prompt   = "--prompt   optional; otherwise will not create a prompting trusonafication";
    char *presence = "--presence optional; otherwise will not prompt for presence";
    char *tilted   = "--tilted   optional; otherwise, will not create a \"tilted\" trusonafication";

    printf("\nOptions:\n  %s\n  %s\n  %s\n  %s\n  %s\n\n", user, settings, prompt, presence, tilted);

    return(TRUSONA_INIT_ERROR);
  }

  enum TRUSONA_SDK_RESULT result = TRUSONA_INSUFFICIENT;

  char *user_identifier = NULL;
  bool  presence        = FALSE;
  bool  prompt          = FALSE;
  bool  tilted          = FALSE;
  int   idx             = -1;

  static struct option options[] =
  {
    { "prompt",   no_argument,       NULL, 0 },
    { "tilted",   no_argument,       NULL, 0 },
    { "presence", no_argument,       NULL, 0 },
    { "user",     required_argument,    0, 0 },
  };

  while (getopt_long_only(argc, argv, "", options, &idx) != -1)
  {
    if (strcmp("presence", options[idx].name) == 0) {
      presence = TRUE;
      continue;
    }

    if (strcmp("tilted", options[idx].name) == 0) {
      tilted = TRUE;
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

    if (tilted) {
      result = trusonafy_v2_ext(json_settings, user_identifier, prompt, presence);
    }
    else {
      result = trusonafy_v1(json_settings, user_identifier);
    }
  }

  return(result);
}
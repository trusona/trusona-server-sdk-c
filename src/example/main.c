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

#include <getopt.h>

#include "../trusona.h"
//
// OR the following after "make install"
//
// #include <trusona/trusona.h>
//

const char *default_json_settings = "/usr/local/etc/trusona/settings.json";

// tilted options: --settings <path_to_json_settings> --user <any_user_identifier> --prompt --presence --tilted

int main(int argc, char *argv[])
{
  if (argc <= 1) {
    char *user     = "--user <any_user_identifier> - required; user to send trusonafication to";
    char *settings = "--settings <path_to_json_settings> - optional; otherwise will use the default /usr/local/etc/trusona/settings.json";
    char *prompt   = "--prompt   optional; otherwise will not create a prompting trusonafication";
    char *presence = "--presence optional; otherwise will not prompt for presence";
    char *tilted   = "--tilted   optional; otherwise will not create a \"tilted\" trusonafication";

    printf("\nOptions:\n  %s\n  %s\n  %s\n  %s\n  %s\n\n", user, settings, prompt, presence, tilted);

    return(TRUSONA_INIT_ERROR);
  }

  enum TRUSONA_SDK_RESULT result = TRUSONA_INSUFFICIENT;

  char *user_identifier = NULL;
  char *settings_path   = (char *)default_json_settings;
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
    { "settings", required_argument,    0, 0 }
  };

  int c;

  while ((c = getopt_long_only(argc, argv, "", options, &idx)) != -1)
  {
    if (c < 0 || c > (sizeof(options) - 1)) {
      continue;
    }

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

    if (strcmp("settings", options[idx].name) == 0) {
      settings_path = optarg;
      continue;
    }
  }

  if (user_identifier != NULL) {
    printf("Sending trusonafication to '%s'\n", user_identifier);
    printf("JSON settings will load from %s\n", settings_path);

    if (tilted) {
      result = trusonafy_v2_ext(settings_path, user_identifier, prompt, presence);
    }
    else {
      result = trusonafy_v1(settings_path, user_identifier);
    }
  }

  return(result);
}
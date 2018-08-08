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

#include "pam_trusona.h"

const char *default_settings = "/usr/local/etc/trusona/settings.json";

int pam_trusona(pam_handle_t *pam, int flags, int argc, const char **argv)
{
  const char *user_identifier = NULL;
  const char *settings        = NULL;
  const char *domain          = NULL;
  const char *username        = NULL;

  bool presence = FALSE;
  bool prompt   = FALSE;
  bool tilted   = FALSE;
  int  i;

  struct passwd *pwd;
  struct key_value
  {
    char *key;
    char *value;
  };

  pam_get_user(pam, &username, NULL);
  syslog(LOG_NOTICE, "%s: System username is '%s'", TRUSONA_LIB, username);

  for (i = 0; i < argc; i++) {
    char *value = (char *)argv[i];

    if (value != NULL) {
      syslog(LOG_DEBUG, "%s: configured key-value pair: %s", TRUSONA_LIB, value);
      struct key_value kv;

      kv.key   = strtok(value, "=");
      kv.value = strtok(NULL, "=");

      if (kv.key != NULL && kv.value != NULL) {
        syslog(LOG_DEBUG, "%s: parsed key-value pair: '%s' <-> '%s'", TRUSONA_LIB, kv.key, kv.value);

        if (strcmp(kv.key, "settings") == 0) {
          settings = kv.value;
        }
        else if (strcmp(kv.key, "domain") == 0) {
          domain = kv.value;
        }
        else if (strcmp(kv.key, "presence") == 0) {
          presence = strcmp(kv.value, "yes") == 0;
        }
        else if (strcmp(kv.key, "tilted") == 0) {
          tilted = strcmp(kv.value, "yes") == 0;
        }
        else if (strcmp(kv.key, "prompt") == 0) {
          prompt = strcmp(kv.value, "yes") == 0;
        }
      }
    }
  }

  settings = settings == NULL ? default_settings : settings;

  if ((pwd = getpwnam(username)) != NULL) {
    user_identifier = configured_user_identifier(pwd->pw_dir);
  }

  if (user_identifier == NULL && domain != NULL) {
    user_identifier = concat_str(username, concat_str("@", domain));
  }

  enum TRUSONA_SDK_RESULT rc;

  if (tilted) {
    rc = trusonafy_v2_ext(settings, user_identifier, prompt, presence);
  }
  else {
    rc = trusonafy_v1(settings, user_identifier);
  }

  if (rc == TRUSONA_SUCCESS) {
    syslog(LOG_NOTICE, "%s: Trusonafication succeeded!", TRUSONA_LIB);
    return(PAM_SUCCESS);
  }
  else {
    syslog(LOG_ERR, "%s: Trusonafication failed!", TRUSONA_LIB);
    return(PAM_AUTH_ERR);
  }
}

/* PAM entry point for authentication - http://linux.die.net/man/3/pam_authenticate */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(pam_trusona(pamh, flags, argc, argv));
}

/* PAM entry point for session creation - http://linux.die.net/man/3/pam_open_session */
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(pam_trusona(pamh, flags, argc, argv));
}

/* PAM entry point for session cleanup - http://linux.die.net/man/3/pam_close_session */
PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_SUCCESS);
}

/* PAM entry point for accounting - http://linux.die.net/man/3/pam_acct_mgmt */
PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(PAM_SUCCESS);
}

// PAM entry point for setting user credentials (that is, to actually establish the authenticated
// user's credentials to the service provider) - http://linux.die.net/man/3/pam_setcred
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(pam_trusona(pamh, flags, argc, argv));
}

/* PAM entry point for authentication token changes - http://linux.die.net/man/3/pam_chauthtok */
PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return(pam_trusona(pamh, flags, argc, argv));
}
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

#include "trusona_pam.h"

const char *pam_module_name = "trusona_pam";
const char *default_settings = "/usr/local/etc/trusona/settings.json";

int trusona_pam(pam_handle_t *pam, int flags, int argc, const char **argv) {
  const char *settings = NULL;
  const char *username;
  int i;

  struct key_value {
    char *key;
    char *value;
  };

  pam_get_user(pam, &username, NULL);
  syslog(LOG_NOTICE, "%s: System username is '%s'", pam_module_name, username);

  for (i = 0; i < argc; i++) {
    char *value = (char *)argv[i];

    if(value != NULL) {
      syslog(LOG_NOTICE, "%s: configured key-value pair: %s", pam_module_name, value);
      struct key_value kv;

      kv.key = strtok(value, "=");
      kv.value = strtok(NULL, "=");

      if(kv.key != NULL && kv.value != NULL) {
        syslog(LOG_NOTICE, "%s: parsed key-value pair: %s:%s", pam_module_name, kv.key, kv.value);

        if (!settings && strcmp(kv.key, "settings") == 0) {
          settings = kv.value;
        }
      }
    }
  }

  settings = settings == NULL ? default_settings : settings;

  enum TRUSONA_SDK_RESULT rc = trusonafy(settings, username);

  if(rc == TRUSONA_SUCCESS) {
    fprintf(stderr, "\nTrusonafication succeeded!\n");
    return PAM_SUCCESS;
  }
  else {
    fprintf(stderr, "\nTrusonafication failed!\n");
    return PAM_AUTH_ERR;
  }
}

/* PAM entry point for authentication - http://linux.die.net/man/3/pam_authenticate */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return trusona_pam(pamh, flags, argc, argv);
}

/* PAM entry point for session creation - http://linux.die.net/man/3/pam_open_session */
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return PAM_IGNORE;
}

/* PAM entry point for session cleanup - http://linux.die.net/man/3/pam_close_session */
PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return PAM_SUCCESS;
}

/* PAM entry point for accounting - http://linux.die.net/man/3/pam_acct_mgmt */
PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return PAM_SUCCESS;
}

// PAM entry point for setting user credentials (that is, to actually establish the authenticated
// user's credentials to the service provider) - http://linux.die.net/man/3/pam_setcred
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return PAM_SUCCESS;
}

/* PAM entry point for authentication token changes - http://linux.die.net/man/3/pam_chauthtok */
PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return trusona_pam(pamh, flags, argc, argv);
}
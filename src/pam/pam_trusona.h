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

#ifndef PAM_TRUSONA_H
#define PAM_TRUSONA_H

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <utmpx.h>
#include <sys/types.h>
#include <pwd.h>

#include "config.h"
#include "pam_internal.h"
#include "../trusona.h"

#if defined(HAVE_SECURITY_PAM_APPL_H)
#include <security/pam_appl.h>
#elif defined(HAVE_PAM_PAM_APPL_H)
#include <pam/pam_appl.h>
#endif

#if defined(HAVE_SECURITY_PAM_MODULES_H)
#include <security/pam_modules.h>
#elif defined(HAVE_PAM_PAM_APPL_H)
#include <pam/pam_modules.h>
#else
# error security/pam_modules.h or pam/pam_modules.h required
#endif

/*************************************************************************
* Platform specific defines
*************************************************************************/

#ifndef CONST
#  if defined(__sun) || defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)

/*
 *  On older versions of Solaris, you may have to change this to:
 *  #define CONST
 */
#    define CONST    const
#  else
#    define CONST
#  endif
#endif

#ifndef PAM_EXTERN
#  ifdef __sun
#    define PAM_EXTERN    extern
#  else
#    define PAM_EXTERN
#  endif
#endif

/*************************************************************************
* Useful macros and defines
*************************************************************************/

#define _pam_forget(X)    if (X) { memset(X, 0, strlen(X)); free(X); X = NULL; }
#ifndef _pam_drop
#define _pam_drop(X)      if (X) { free(X); X = NULL; }
#endif

#define PAM_DEBUG_ARG         1
#define PAM_SKIP_PASSWD       2
#define PAM_USE_FIRST_PASS    4
#define PAM_TRY_FIRST_PASS    8
#define PAM_RUSER_ARG         16

#ifndef BUFFER_SIZE
#define BUFFER_SIZE           1024
#endif

#ifndef PAM_SM_AUTH
#define PAM_SM_AUTH
#endif

#ifndef PAM_SM_PASSWORD
#define PAM_SM_PASSWORD
#endif

#ifndef PAM_SM_SESSION
#define PAM_SM_SESSION
#endif

#endif /* TRUSONA_PAM_H */
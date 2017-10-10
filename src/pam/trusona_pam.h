/*
 * MIT License
 *
 * Copyright (c) 2016, 2017 Trusona Inc (www.trusona.com) All Rights Reserved
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

 #ifndef TRUSONA_PAM_H
#define TRUSONA_PAM_H

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
#include <utmp.h>

#include "config.h"

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

int trusona_pam_authenticate(pam_handle_t *pam, int flags, int argc, const char **argv);

/*************************************************************************
* Platform specific defines
*************************************************************************/

#ifndef CONST
#  if defined(__sun) || defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
/*
 *  On older versions of Solaris, you may have to change this to:
 *  #define CONST
 */
#    define CONST const
#  else
#    define CONST
#  endif
#endif

#ifndef PAM_EXTERN
#  ifdef __sun
#    define PAM_EXTERN extern
#  else
#    define PAM_EXTERN
#  endif
#endif


/*************************************************************************
* Useful macros and defines
*************************************************************************/

#define _pam_forget(X) if (X) {memset(X, 0, strlen(X)); free(X); X = NULL; }
#ifndef _pam_drop
#define _pam_drop(X) if (X) {free(X); X = NULL; }
#endif

#define PAM_DEBUG_ARG      1
#define PAM_SKIP_PASSWD    2
#define PAM_USE_FIRST_PASS 4
#define PAM_TRY_FIRST_PASS 8
#define PAM_RUSER_ARG      16

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
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
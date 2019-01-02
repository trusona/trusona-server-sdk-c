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

#ifndef TRUSONA_H
#define TRUSONA_H

#include <uuid/uuid.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <jansson.h>
#include <stdbool.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

#include "session.h"
#include "internal.h"
#include "json.h"
#include "net.h"
#include "hmac.h"
#include "str_utils.h"
#include "file_utils.h"
#include "trusonafication.h"

#ifndef TRUSONA_MAX_WAIT
#define TRUSONA_MAX_WAIT    99 // 99 seconds
#endif

#ifndef MIN_LOA_LEVEL
#define MIN_LOA_LEVEL    1
#endif

#ifndef MAX_LOA_LEVEL
#define MAX_LOA_LEVEL    4
#endif

#ifndef TRUSONA_SLEEP
#define TRUSONA_SLEEP    5 // 5 seconds
#endif

#ifndef MAX_STR
#define MAX_STR    256
#endif

#ifndef FALSE
#define FALSE    0
#undef TRUE
#define TRUE     !FALSE
#endif

#ifndef TRUSONA_LIB
#define TRUSONA_LIB    "TRUSONA"
#endif

const enum TRUSONA_SDK_RESULT trusonafy_v1(const char *json_settings, const char *email_or_trusona_id);

// default; with prompt and with presence
const enum TRUSONA_SDK_RESULT trusonafy_v2(const char *json_settings, const char *user_identifier);

const enum TRUSONA_SDK_RESULT trusonafy_v2_ext(const char *json_settings, const char *user_identifier, const bool prompt, const bool presence);

#endif // TRUSONA_H
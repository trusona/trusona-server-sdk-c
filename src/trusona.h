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

#ifndef TRUSONA_H
#define TRUSONA_H

#include <uuid/uuid.h>
#include <curl/curl.h>
#include <jansson.h>
#include <stdbool.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "settings.h"
#include "utils.h"
#include "json.h"
#include "net.h"
#include "hmac.h"

#ifndef TRUSONA_MAX_WAIT
#define TRUSONA_MAX_WAIT 99 // 99 seconds
#endif

#ifndef MIN_LOA_LEVEL
#define MIN_LOA_LEVEL 1
#endif

#ifndef MAX_LOA_LEVEL
#define MAX_LOA_LEVEL 4
#endif

#ifndef TRUSONA_SLEEP
#define TRUSONA_SLEEP 5 // 5 seconds
#endif

#ifndef MAX_STR
#define MAX_STR 1024
#endif

#ifndef FALSE
#define FALSE 0
#undef TRUE
#define TRUE !FALSE
#endif

const enum API_INPUT_TYPE get_api_input_type(const char* value);

const enum TRUSONA_SDK_RESULT trusonafy(const char* json_settings, const char* email_or_trusona_id);

#endif /* TRUSONA_H */
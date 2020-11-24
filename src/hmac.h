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

#ifndef TRUSONA_HMAC_H
#define TRUSONA_HMAC_H

#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <string.h>

#include "str_utils.h"
#include "base64.h"

#ifndef MAX_STR
#define MAX_STR    1024
#endif

char *generate_md5(const char *data);

char *base64_hmac_sha256(const char *key, const char *data);

#endif // TRUSONA_HMAC_H
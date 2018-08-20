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

#ifndef TRUSONA_INTERNAL_H
#define TRUSONA_INTERNAL_H

#include "trusona.h"

#ifndef VALID_REQ
#define VALID_REQ 0
#endif

#ifndef INVALID_REQ
#define INVALID_REQ -1
#endif

struct MemoryStruct {
  char *data;
  size_t size;
};

char *concat_str(const char *arr1, const char *arr2);

void append_str(char **buffer, const char *data);

TrusonaSession load_settings(const char *json_settings_file);

char *now_rfc1123();

char *rfc8601(int from_now_seconds);

#endif

/* TRUSONA_INTERNAL_H */

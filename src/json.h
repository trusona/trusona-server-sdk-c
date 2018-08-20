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

#ifndef TRUSONA_JSON_H
#define TRUSONA_JSON_H

#include "trusona.h"

const char *json_str_value(char **json, const char *data);

const int  json_int_value(char **json, const char *data);

const int  get_int_value(json_t  *root, const char  *data);

const char *get_str_value(json_t  *root, const char  *data);

const json_t *get_object(char **json, const char *key);

#endif /* TRUSONA_JSON_H */

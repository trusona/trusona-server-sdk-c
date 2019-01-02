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

#ifndef TRUSONAFICATION_H
#define TRUSONAFICATION_H

#include "trusona.h"

enum TRUSONA_INPUT
{
  EMAIL_ADDRESS,
  TRUSONA_ID,
  USER_IDENTIFIER,
  UNKNOWN
};

enum TRUSONA_SDK_RESULT
{
  TRUSONA_SUCCESS,
  TRUSONA_INSUFFICIENT,
  TRUSONA_FAILURE,
  TRUSONA_SERVICE_ERROR,
  TRUSONA_INIT_ERROR
};

const enum TRUSONA_SDK_RESULT trusonafy(TrusonaSession trusona_session);

#endif // TRUSONAFICATION_H
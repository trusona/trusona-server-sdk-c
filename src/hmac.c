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

#include "hmac.h"

static const unsigned int SHA256_BITS = 32;
static const unsigned int MD5_BITS    = 16;

char *generate_md5(const char *data)
{
  if (data == NULL) {
    return(NULL);
  }

  unsigned int   i;
  unsigned char *raw_digest;
  char *         hex_digest;

  hex_digest = calloc(1, sizeof(char) * MAX_STR);
  raw_digest = MD5((const unsigned char *)data, strnlen((const char *)data, MAX_STR), NULL);

  for (i = 0; i < MD5_BITS; i++) {
    snprintf((char *)&hex_digest[i * 2], MAX_STR, "%02x", (unsigned int)raw_digest[i]);
  }

  raw_digest = NULL;

  return(hex_digest);
}

char *base64_hmac_sha256(const char *key, const char *data)
{
  if (key == NULL || data == NULL || strnlen((char *)trim(key), MAX_STR) == 0) {
    return(NULL);
  }

  unsigned char *digest;
  char *         hex_digest;
  char *         base64_val;
  int            i;

  digest     = HMAC(EVP_sha256(), key, strnlen(key, MAX_STR), (const unsigned char *)data, strnlen((char *)data, MAX_STR), NULL, NULL);
  hex_digest = calloc(1, sizeof(char) * MAX_STR);

  for (i = 0; i < SHA256_BITS; i++) {
    snprintf((char *)&hex_digest[i * 2], MAX_STR, "%02x", (unsigned int)digest[i]);
  }

  base64_val = calloc(1, sizeof(char) * MAX_STR);

  Base64encode(base64_val, hex_digest, strnlen(hex_digest, MAX_STR));

  hex_digest = NULL;
  digest     = NULL;

  return(base64_val);
}
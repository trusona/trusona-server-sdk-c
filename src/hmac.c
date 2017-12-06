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

#include "hmac.h"
#include "base64.h"

static const unsigned int SHA256_BITS = 32;
static const unsigned int MD5_BITS = 16;


char *generate_md5(const char *data) {
  unsigned char *raw_digest;
  int i;

  char *hex_digest = calloc(1, sizeof(char)  *MAX_STR);
  raw_digest = MD5((const unsigned char *)data, strnlen((const char *)data, MAX_STR), NULL);

  for(i = 0; i < MD5_BITS; i++) {
    snprintf((char*)&hex_digest[i*2], MAX_STR, "%02x", (unsigned int)raw_digest[i]);
  }

  raw_digest = NULL;

  return hex_digest;
}

char *base64_hmac_sha256(const char *key, const char *data) {
  unsigned char *digest;
  char *hex_digest;
  char *base64_val;
  int i;

  digest = HMAC(EVP_sha256(), key, strnlen(key, MAX_STR), (const unsigned char *)data, strnlen((char *)data, MAX_STR), NULL, NULL);
  hex_digest = calloc(1, sizeof(char)  *MAX_STR);

  for(i = 0; i < SHA256_BITS; i++) {
    snprintf((char*)&hex_digest[i*2], MAX_STR, "%02x", (unsigned int)digest[i]);
  }

  base64_val = calloc(1, sizeof(char)  *MAX_STR);

  Base64encode(base64_val, hex_digest, strnlen(hex_digest, MAX_STR));

  hex_digest = NULL;
  digest = NULL;

  return base64_val;
}
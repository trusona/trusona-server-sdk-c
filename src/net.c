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

#include "net.h"

static bool curl_is_ready = false;

static const char *lib_module_name = "trusona";

static const char *ENDL = "\n";
static const char *COLON = ":";
static const char *SPACE = " ";
static const char *BLANK = "";

static const char *JSON_TYPE = "application/json";
static const char *POST = "POST";
static const char *GET = "GET";

static void init_curl() {
  if(!curl_is_ready) {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_is_ready = true;
  }
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *ptr) {
  struct MemoryStruct *mem = (struct MemoryStruct *)ptr;
  size_t realsize = size  *nmemb;

  mem->data = realloc(mem->data, mem->size + realsize + 1);

  if(mem->data == NULL) {
    printf("Error: Not enough data (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->data[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->data[mem->size] = 0;

  return realsize;
}


int do_get_request(SettingsStruct settings, const char *uri, char **json) {
  CURL *curl;
  CURLcode code;
  long status;

  init_curl();
  curl = curl_easy_init();

  if(curl) {
    struct curl_slist *headers = NULL;
    struct MemoryStruct output;

    output.data = calloc(1, sizeof(char)  *1);
    output.size = 0;

    char *url = concat_str(settings.api_host, uri);
    char *md5_hash = "d41d8cd98f00b204e9800998ecf8427e"; // MD5 hash of "" (aka nothing)
    char *hmac_parts = calloc(1, sizeof(char) * MAX_STR);
    char *now = now_rfc1123();

    if(hmac_parts != NULL) {
      append_str(&hmac_parts, GET);
      append_str(&hmac_parts, ENDL);
      append_str(&hmac_parts, md5_hash);
      append_str(&hmac_parts, ENDL);
      append_str(&hmac_parts, BLANK);
      append_str(&hmac_parts, ENDL);

      if(now != NULL) {
        append_str(&hmac_parts, now);
      }

      append_str(&hmac_parts, ENDL);
      append_str(&hmac_parts, uri);
    }

    char *signature = base64_hmac_sha256(settings.mac_key, hmac_parts);
    char *auth_header = calloc(1, sizeof(char) * MAX_STR);

    append_str(&auth_header, settings.token_type);
    append_str(&auth_header, SPACE);
    append_str(&auth_header, settings.access_token);
    append_str(&auth_header, COLON);
    append_str(&auth_header, signature);

    headers = curl_slist_append(headers, concat_str("x-request-id: ", settings.request_id));
    headers = curl_slist_append(headers, concat_str("authorization: ", auth_header));
    headers = curl_slist_append(headers, concat_str("date: ", now));

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Trusona C API/0.1");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&output);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    code = curl_easy_perform(curl);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

    if(code == CURLE_OK && (status == 200 || status == 201)) {
      syslog(LOG_NOTICE, "%s: %s: Successful GET to %s", lib_module_name, settings.request_id, url);

      int cnt = output.size + 1;
      *json = calloc(cnt, sizeof(char)  *cnt);
      strncpy(*json, output.data, cnt);
    }
    else {
      syslog(LOG_NOTICE, "%s: %s: Failed GET to %s", lib_module_name, settings.request_id, url);
      *json = NULL;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    free(auth_header);
    free(hmac_parts);
    free(signature);
    free(url);
    free(now);

    auth_header = hmac_parts = signature = NULL;
    md5_hash = url = now = NULL;
  }

  curl_global_cleanup();

  return *json == NULL ? INVALID_REQ : VALID_REQ;
}


int do_post_request(SettingsStruct settings, const char *uri, const char *post_data, char **json) {
  CURL *curl;
  CURLcode code;
  long status;

  init_curl();
  curl = curl_easy_init();

  if(curl) {
    struct curl_slist *headers = NULL;
    struct MemoryStruct output;

    output.data = calloc(1, sizeof(char)  *1);
    output.size = 0;

    char *url = concat_str(settings.api_host, uri);
    char *md5_hash = generate_md5(post_data);
    char *now = now_rfc1123();

    char *hmac_parts = calloc(1, sizeof(char) * MAX_STR);

    if(hmac_parts != NULL) {
      append_str(&hmac_parts, POST);
      append_str(&hmac_parts, ENDL);
      append_str(&hmac_parts, md5_hash);
      append_str(&hmac_parts, ENDL);
      append_str(&hmac_parts, JSON_TYPE);
      append_str(&hmac_parts, ENDL);
      append_str(&hmac_parts, now);
      append_str(&hmac_parts, ENDL);
      append_str(&hmac_parts, uri);
    }

    char *signature = base64_hmac_sha256(settings.mac_key, hmac_parts);
    char *auth_header = calloc(1, sizeof(char) * MAX_STR);

    append_str(&auth_header, settings.token_type);
    append_str(&auth_header, SPACE);
    append_str(&auth_header, settings.access_token);
    append_str(&auth_header, COLON);
    append_str(&auth_header, signature);

    headers = curl_slist_append(headers, concat_str("x-request-id: ", settings.request_id));
    headers = curl_slist_append(headers, concat_str("authorization: ", auth_header));
    headers = curl_slist_append(headers, concat_str("content-type: ", JSON_TYPE));
    headers = curl_slist_append(headers, concat_str("date: ", now));

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Trusona C API/0.1");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&output);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    code = curl_easy_perform(curl);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

    if(code == CURLE_OK && (status == 200 || status == 201)) {
      syslog(LOG_NOTICE, "%s: %s: Successful POST to %s", lib_module_name, settings.request_id, url);

      int cnt = output.size + 1;
      *json = calloc(cnt, sizeof(char)  *cnt);

      strncpy(*json, output.data, cnt);
    }
    else {
      syslog(LOG_NOTICE, "%s: %s: Failed POST to %s", lib_module_name, settings.request_id, url);
      *json = NULL;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    free(auth_header);
    free(hmac_parts);
    free(signature);
    free(md5_hash);
    free(url);
    free(now);

    auth_header = hmac_parts = signature = NULL;
    md5_hash = url = now = NULL;
  }

  curl_global_cleanup();

  return *json == NULL ? INVALID_REQ : VALID_REQ;
}

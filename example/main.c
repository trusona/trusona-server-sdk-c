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

#include <trusona/trusona.h>
#include <ctype.h>

const char* settings = "/usr/local/etc/trusona/settings-hauz.json";
char * trim(const char *str);

int main() {
  char* trimmed_value = NULL;
  char* value;

  printf("Enter a trusona ID or an email address: ");
  value = calloc(1, sizeof(char) * MAX_STR);

  if(value != NULL) {
    trimmed_value = trim(fgets(value, MAX_STR, stdin));

    printf("Sending trusonafication to '%s'\n", trimmed_value);
    printf("JSON settings will load from %s\n", settings);

    enum TRUSONA_SDK_RESULT result = trusonafy(settings, trimmed_value);
    printf("%d\n", result);
  }

  free(trimmed_value);
  free(value);

  trimmed_value = value = NULL;

  return 0;

}

char *trim(const char *str) {
  char *ptr = calloc(1, sizeof(char) * MAX_STR);

  int first = -1;
  int last = strnlen(str, MAX_STR);

  while(isspace(str[++first])) {
    // do nothing
  }

  while(isspace(str[--last])) {
    // do nothing
  }

  int idx = 0;

  if(first != last) {
    for(; first <= last; ) {
      ptr[idx++] = str[first++];
    }
  }

  ptr[idx] = '\0';

  return ptr;
}
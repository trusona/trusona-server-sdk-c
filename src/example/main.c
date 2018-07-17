/*
 * MIT License
 *
 * Copyright (c) 2016, 2017, 2018 Trusona Inc (www.trusona.com) All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../trusona.h"

const char *json_settings = "/usr/local/etc/trusona/settings.json";

// tilted options: --user=<any_user_identifier> --prompt=yes  and/or --presence=yes

int main(int argc, char *argv[])
{
  enum TRUSONA_SDK_RESULT result = TRUSONA_INSUFFICIENT;
  char *value = NULL;

  if (argc != 2) {
    printf("Enter a trusona ID or an email address: ");
    value = calloc(1, sizeof(char) * MAX_STR);
    value = trim(fgets(value, MAX_STR, stdin));
  }
  else {
    value = trim(argv[1]);
  }

  if (value != NULL) {
    printf("Sending trusonafication to '%s'\n", value);
    printf("JSON settings will load from %s\n", json_settings);

    result = trusonafy_v2(json_settings, value);
  }

  return(result);
}
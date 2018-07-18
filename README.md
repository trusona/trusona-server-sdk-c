# Trusona C SDK

[![Build Status](https://travis-ci.com/lighthauz/trusona-ceee-sdk.svg?token=ERoqgs7tKf7xAGTsABr8&branch=master)](https://travis-ci.com/lighthauz/trusona-ceee-sdk)


## Usage

```c
const enum TRUSONA_SDK_RESULT trusonafy_v2(const char* path_to_json_settings, const char* valid_user_identifier);
```

Alternatively,

```c
const enum TRUSONA_SDK_RESULT trusonafy_v2_ext(const char* path_to_json_settings, const char* valid_user_identifier, const bool prompt, const bool user_presence);
```

#### Deprecated Usage:

```c
const enum TRUSONA_SDK_RESULT trusonafy_v1(const char* path_to_json_settings, const char* email_or_trusona_id);
```


`TRUSONA_SDK_RESULT` is an `enum` declared as follows:

```c
enum TRUSONA_SDK_RESULT {
  TRUSONA_SUCCESS,        // good
  TRUSONA_INIT_ERR        // bad
  TRUSONA_SERVICE_ERR,    // bad
  TRUSONA_INSUFFICIENT,   // bad
  TRUSONA_FAILURE         // bad
};
```

A value of `TRUSONA_SUCCESS` indicates trusonafication was successful.


### example `main.c`

```c
#include <trusona/trusona.h>

const char* settings = "/path/to/trusona/settings.json";

int main() {
  enum TRUSONA_SDK_RESULT result = TRUSONA_INSUFFICIENT;
  char* trimmed_value = NULL;
  char* value;

  printf("Enter a trusona ID or an email address: ");
  value = calloc(1, sizeof(char) * MAX_STR);

  if(value != NULL) {
    trimmed_value = trim(fgets(value, MAX_STR, stdin));

    printf("Sending trusonafication to '%s'\n", trimmed_value);
    printf("JSON settings will load from %s\n", settings);

    result = trusonafy_v1(settings, trimmed_value);
  }

  free(trimmed_value);
  free(value);

  trimmed_value = value = NULL;

  return result;
}
```

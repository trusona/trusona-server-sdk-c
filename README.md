# Trusona C SDK

[![Build Status](https://travis-ci.com/lighthauz/trusona-ceee-sdk.svg?token=ERoqgs7tKf7xAGTsABr8&branch=master)](https://travis-ci.com/lighthauz/trusona-ceee-sdk)


## Usage

```c
const enum TRUSONA_SDK_RESULT trusonafy_v2(const char* path_to_json_settings, const char* valid_user_identifier);
```

Alternatively, for extended usage:

```c
const enum TRUSONA_SDK_RESULT trusonafy_v2_ext(const char* path_to_json_settings, const char* valid_user_identifier, const bool prompt, const bool user_presence);
```

##### Deprecated Usage:

```c
const enum TRUSONA_SDK_RESULT trusonafy_v1(const char* path_to_json_settings, const char* email_or_trusona_id);
```


`TRUSONA_SDK_RESULT` is an `enum` declared as follows:

```c
enum TRUSONA_SDK_RESULT {
  TRUSONA_SUCCESS,          // what you want to see!
  TRUSONA_INSUFFICIENT,     // the trusonafication was rejected
  TRUSONA_FAILURE,          // other API error
  TRUSONA_SERVICE_ERROR,    // API service error
  TRUSONA_INIT_ERROR        // failure to initialize the API
};
```

A value of `TRUSONA_SUCCESS` indicates trusonafication was successful.


## Example 

See [src/example/main.c](https://github.com/lighthauz/trusona-ceee-sdk/blob/master/src/example/main.c) for a functional implementation.

## TODO

- Add debuging output option.





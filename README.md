# Trusona C SDK

[![Build Status](https://travis-ci.com/lighthauz/trusona-ceee-sdk.svg?token=ERoqgs7tKf7xAGTsABr8&branch=master)](https://travis-ci.com/lighthauz/trusona-ceee-sdk)


## Usage

```c
const enum TRUSONA_SDK_RESULT trusonafy(const char* path_to_json_settings, const char* email_or_trusona_id);
```

`TRUSONA_SDK_RESULT` is an `enum` declared as follows:

```c
enum TRUSONA_SDK_RESULT {  
  TRUSONA_SUCCESS,            // good
  TRUSONA_SERVICE_ERR,        // bad
  TRUSONA_CONV_ERR,           // bad
  TRUSONA_AUTH_ERR,           // bad
  TRUSONA_CRED_INSUFFICIENT   // bad
};
```

A value of `TRUSONA_SUCCESS` indicates trusonafication was successful.

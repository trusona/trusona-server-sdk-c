# Trusona C SDK

[![Build Status](https://travis-ci.com/lighthauz/trusona-ceee-sdk.svg?token=ERoqgs7tKf7xAGTsABr8&branch=master)](https://travis-ci.com/lighthauz/trusona-ceee-sdk)

## Dependencies

- [libcurl](https://curl.haxx.se/libcurl/c) ver. 7.61.0, for interweb communication
- [jansson](http://www.digip.org/jansson) ver. [2.11](https://jansson.readthedocs.io/en/2.11) for JSON manipulation
- [openssl](https://www.openssl.org) ver. [1.0.2o](https://www.openssl.org/source) for cryptographic functions
- [ossp-uuid](https://github.com/sean-/ossp-uuid) - or equivalent provider of `uuid/uuid.h` - for UUID v4 generation
- [PAM](http://tldp.org/HOWTO/User-Authentication-HOWTO/x115.html) - optional. If you do not require PAM support, delete `src/pam` before running `make`


## Building & Installation

- With all the dependencies in place, `make all` should complete successfully.
- By default, `make install` will install the shared library into `/usr/local/lib`.
  - Development headers will be installed into `/usr/local/include`


## Usage

```c
const enum TRUSONA_SDK_RESULT trusonafy_v2(const char* path_to_json_settings,
                                            const char* valid_user_identifier);
```

Alternatively, for extended usage:

```c
const enum TRUSONA_SDK_RESULT trusonafy_v2_ext(const char* path_to_json_settings,
                                                const char* valid_user_identifier,
                                                const bool prompt,
                                                const bool user_presence);
```

##### Legacy Usage:

```c
const enum TRUSONA_SDK_RESULT trusonafy_v1(const char* path_to_json_settings,
                                            const char* email_or_trusona_id);
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

### TODO

- Add debuging output option.

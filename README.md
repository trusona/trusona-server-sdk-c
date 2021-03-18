# Trusona C SDK

[![Build Status](https://travis-ci.com/trusona/trusona-server-sdk-c.svg?token=ERoqgs7tKf7xAGTsABr8&branch=master)](https://travis-ci.com/trusona/trusona-server-sdk-c)

## Build Tools

You will need a [C](https://en.wikipedia.org/wiki/C_programming_language) language compiler like [gcc](https://github.com/gcc-mirror/gcc) or [clang](https://github.com/llvm-mirror/clang).

## Dependencies

- [libcurl](https://curl.haxx.se/libcurl/c) ver. 7.61.0, for interweb communication
- [jansson](https://github.com/akheron/jansson) ver. [2.12](https://github.com/akheron/jansson/releases) for JSON manipulation
- [openssl](https://www.openssl.org) ver. [1.1.1](https://www.openssl.org/source) for cryptographic functions
- [ossp-uuid](https://github.com/sean-/ossp-uuid) - or equivalent provider of `uuid/uuid.h` - for UUID v4 generation
- [PAM](http://tldp.org/HOWTO/User-Authentication-HOWTO/x115.html) - optional. If you do not require PAM support, delete `src/pam` before running any `make` commands

### Test Dependencies

To successfully run the tests via `make test`, you will need to have the [check](https://libcheck.github.io/check) and [subunit](https://github.com/testing-cabal/subunit/tree/master/c) libraries in your environment.

## Building & Installation

With all the dependencies in place, `make all` should complete successfully.

By default, `[sudo] make install` will install the shared libraries into `/usr/local/lib`.

Development headers will be installed into `/usr/local/include/trusona`

## Debian Ubuntu Package Installation

Debian Ubuntu packages are made available on the [releases](https://github.com/trusona/trusona-server-sdk-c/releases) page.

The packages are currently built for `Ubuntu 20.04` aka [Focal Fossa](https://releases.ubuntu.com/20.04).

The following commands will download and prompt for the installation of the binary including all dependencies.


### Install using `wget`

```bash
wget -q https://github.com/trusona/trusona-server-sdk-c/releases/download/1.0.0/trusona-server-sdk_1.0.0-1_amd64.deb -O trusona-server-sdk.deb
sudo apt-get update
sudo apt-get install ./trusona-server-sdk.deb
```

### Install using `curl`

```bash
curl -sL https://github.com/trusona/trusona-server-sdk-c/releases/download/1.0.0/trusona-server-sdk_1.0.0-1_amd64.deb -o trusona-server-sdk.deb
sudo apt-get update
sudo apt-get install ./trusona-server-sdk.deb
```

After successful installation, see the [documentation](https://docs.trusona.com/integrations/ssh-linux) for next steps to get it configured.


## API Usage

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

#### Legacy API Usage:

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


## PAM API Usage

See the PAM [configuration](src/pam/README.md) documentation.

## Example Implementation

See [src/example/main.c](src/example/main.c) for a fully functional implementation.

### TODO

- Add debuging output option

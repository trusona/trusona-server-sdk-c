# Trusona PAM Configuration

PAM Configuration varies across Linux & Unix distributions; however the gist of the settings is always the same.

It typically involves:
  - naming an authorization process i.e. `auth`, `account`, `password`, or `session`
  - providing a control flag - `optional` or `required`
  - the name (and path?) of the configured module; and
  - key-value settings if required by the module


## Trusona PAM Settings
Trusona's PAM module has several key-value settings:

- `settings`
  - Optional
  - If provided, expected to be the absolute path to the Trusona settings JSON file
  - If not provided, the default path of `/usr/local/etc/trusona/settings.json` is used

- `tilted`
  - Optional
  - Expected to be either `yes` or `no`
  - If `yes`, a 2nd generation trusonafication will be sent

- `prompt`
  - Optional
  - Expected to be either `yes` or `no`
  - Only valid when `tilted=yes` is also specified
  - If `yes`, trusonafication will require an affirming prompt; otherwise not

- `presence`
  - Optional
  - Expected to be either `yes` or `no`
  - Only valid when `tilted=yes` is also specified
  - If `yes`, trusonafication will require verification of presence, otherwise not

- `domain`
  - Optional
  - Would be a top-level email domain - for instance `example.com`
  - If provided, it is appended to the username to form a valid email address to be sent a trusonafication

## The `settings.json` file
The settings file is a JSON file with a simple object with various properties that are needed by the PAM module.

By default, it is expected to be found at `/usr/local/etc/trusona/settings.json`

| name | type | description | required? |
|------|------|-------------|-----------|
| `access_token` | string | your server SDK token from Trusona | yes |
| `mac_key` | string | your server SDK secret from Trusona | yes |
| `api_host` | string | Base URL for Trusona services. Use `https://api.trusona.net` unless otherwise instructed | yes |
| `desired_level` | integer | Set to `2` for Essential, `3` for Executive | yes |
| `action` | string | The `action` shown to the user in the Trusonafication prompt | yes |
| `resource` | string | The `resource` shown to the user in the Trusonafication prompt | yes |
| `expires_in_x_seconds` | integer | number of seconds before the Trusonafication expires (must be within 0-300) | no |

### Example
```json
{
  "access_token": "real_access_token",
  "mac_key": "real_mac_key",
  "api_host": "https://api.trusona.net",
  "desired_level": 2,
  "expires_in_x_seconds": 99,
  "action": "action",
  "resource": "resource"
}
```

## User Identifier Configuration via `$HOME/.trusona`

An alternative way to specify a user identifier that is different from the username is to create a file at `$HOME/.trusona` and insert a single line of the desired user identifier who owns `$HOME`.

This file must be a regular file - not a symbolic link - and should have octal permissions of `10400` or `10600`.

The file must be owned by the owner of `$HOME`.

At runtime, `getuid()` must equal `geteuid()` as a precaution against any `setuid(uid_t)` bugs.

Additionally, the contents of this file should be less than 128 bytes; otherwise, bytes beyond that count will be ignored.

Finally, if this file is found, and its contents read, concatenation of the value specified by the `domain` setting will not be done.



### Example

```plain
auth required /absolute/path/to/pam_trusona.so  \
  settings=/absolute/path/to/trusona/settings.json  \
    domain=[optional-example.com] prompt=[no|yes] presence=[no|yes] tilted=[no|yes]
```

# Trusona PAM Configuration

PAM Configuration varies across Linux & Unix distributions; however the gist of the settings is always the same.

It typically involves:
  - naming an authorization process i.e. `auth`, `account`, `password`, or `session`
  - providing a control flag - `optional` or `required`
  - the name (and path?) of the configured module; and
  - key-value settings if required by the module


## Trusona PAM Settings
Trusona's PAM module has several key-value settings:

- `domain`
  - Optional
  - Would be a top-level email domain - for instance `example.com`
  - If provided, it is appended to the username to form a valid email address to be sent a trusonafication

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


### Example

```plain
auth required /absolute/path/to/trusona_pam.so  \
  settings=/absolute/path/to/trusona/settings.json  \
    domain=[example.com] prompt=[no|yes] presence=[no|yes] tilted=[no|yes]
```

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


## Alternative User Identifier Configuration via `$HOME`

An alternative way to specify a user identifier that is different from the username is to create a file at `$HOME/.trusona` and insert a single line of the desired user identifier who owns `$HOME`.

This file must be a regular file - not a symbolic link - and should have octal permissions of `10400` or `10600`.

The file must be owned by the owner of `$HOME`.

Additionally, the contents of this file should be less than 128 bytes; otherwise, bytes beyond that count will be ignored.

Finally, if this file is found, and its contents read, concatenation of the value specified by the `domain` setting will not be done.



### Example

```plain
auth required /absolute/path/to/trusona_pam.so  \
  settings=/absolute/path/to/trusona/settings.json  \
    domain=[optional-example.com] prompt=[no|yes] presence=[no|yes] tilted=[no|yes]
```

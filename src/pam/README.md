# Trusona PAM Configuration

PAM Configuration varies across Linux & Unix distributions; however the gist of the settings is always the same.

It typically involves naming an authorization process, providing a control flag, specifying the name of the configured module and key-value settings that are optional.

The authorization process are one of:
- `auth`
- `account`
- `password`
- `session`

## Trusona PAM Settings
Trusona's PAM module has several key-value settings:
- `domain`
- `settings`
- `tilted`
- `prompt`
- `presence`

#### domain
- Optional
- Would be a top-level email domain.
- If provided, it is appended to the username to form a valid email address to be sent a trusonafication

#### settings
- Optional
- Path to the trusona settings JSON file
- If not provided, the default path of `/usr/local/etc/trusona/settings.json` is used

#### tilted
- Optional
- Expected to be either `yes` or `no`
- If `yes`, a 2nd generation trusonafication will be sent

#### prompt
- Optional
- Expected to be either `yes` or `no`
- Only valid when `tilted=yes` is specified
- If `yes`, trusonafication will require an affirming prompt; otherwise not

#### presence
- Optional
- Expected to be either `yes` or `no`
- Only valid when `tilted=yes` is specified
- If `yes`, trusonafication will require verification of presence, otherwise not

### Example

```text
auth required /absolute/path/to/trusona_pam.so domain=[example.com]  \
    settings=/absolute/path/to/settings.json prompt=[no|yes] presence=[no|yes] tilted=[no|yes]
```

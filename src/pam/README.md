# Configuring Trusona C SDK for PAM

This guide assumes a new install and was written for Ubuntu v20 LTS. Some steps may not be required on an existing and configured installation. Other flavors of linux will be similar, but package names, paths, and directories will differ.

## Dependencies

The following dependencies are required to build the Trusona C SDK:

* [libcurl](https://curl.haxx.se/libcurl/c)
* [jansson](https://github.com/akheron/jansson)
* [openssl](https://www.openssl.org)
* [ossp-uuid](https://github.com/sean-/ossp-uuid)

On Ubuntu, they can be installed with:

```bash
sudo apt install build-essential uuid-dev libcurl4-openssl-dev lib-jansson-dev libssl-dev libpam0g-dev
```

### SDK token and secret

To use the Trusona C SDK, you'll need an SDK token and secret.

1. Navigate to https://dashboard.trusona.com
2. Login using Trusona
3. Click "SDK Credentials" in the sidebar navigation
4. Click the "Create Server Credentials" button
5. Save your token and secret in a safe and secure place

## Build the SDK

1. Clone or download the Trusona C SDK
2. Make and install the project

```bash

sudo make && sudo make install

```

## Configuration


### Trusona settings

Create a `trusona` directory in `/usr/local/etc`:

```bash

sudo mkdir /usr/local/etc/trusona

```

Create a file `settings.json` in the `/usr/local/etc` directory:

```bash

touch /usr/local/etc/trusona/settings.json

```

Edit the file to add the following Trusona settings:

```
{
  "access_token": "<your token here>",
  "mac_key": "<your secret here>",
  "api_host": "https://api.trusona.net",
  "desired_level": 2,
  "expires_in_x_seconds": 60,
  "action": "login",
  "resource": "Ubuntu"
}
```

| Setting name         | Recommended value         | Description                                                       |
|----------------------|:--------------------------|-------------------------------------------------------------------|
| access_token         | Your production token     | The Trusona API token you got from https://dashboard.trusona.com  |
| mac_key              | Your production secret    | The Trusona API secret you got from https://dashboard.trusona.com |
| api_host             | `https://api.trusona.net` | Trusona's API host                                                |
| desired_level        | `2`                       | 2 – For Trusona Essential, 3 – For Trusona Executive              |
| expires_in_x_seconds | `60`                      | Trusonafication expiration time in seconds                        |
| action               | `login`                   | The action displayed to the user when authenticating              |
| resource             | SSH server host name      | The resource displayed to the user when authenticating            |


### Trusona PAM configuration

Move the generated `pam_trusona.so` and `libtrusona.so` files from `/usr/local/lib` to `/usr/local`:

```bash

sudo mv /usr/local/lib/pam_trusona.so /usr/lib
sudo mv /usr/local/lib/libtrusona.so /usr/lib
```

Create a new PAM configuration file in `/etc/pam.d/sshd`:

```bash

sudo touch /etc/pam.d/sshd/trusona
```

Edit the file to include the following:

```plaintext

session required /usr/lib/pam_trusona.so settings=/usr/local/etc/trusona/settings.json prompt=yes presence=yes tilted=no
```

To enable Trusona for SSHD, edit `/etc/pam.d/sshd` to include a reference to Trusona.

An abbreviated example of `/etc/pam.d/ssh` with `@include trusona` directive:

```plaintext
# ...snip...

# Create a new session keyring.
session    optional     pam_keyinit.so force revoke

# Standard Un*x session setup and teardown.
@include common-session

@include trusona

# Print the message of the day upon successful login.
# This includes a dynamically generated part from /run/motd.dynamic
# and a static (admin-editable) part from /etc/motd.
session    optional     pam_motd.so  motd=/run/motd.dynamic
session    optional     pam_motd.so noupdate

# ...snip...
```

### User configuration

Each user connecting to the system with SSH needs to define their Trusona username in their home directory. In the following example, we will configure your user.

Create a file named `.trusona` in your home directory:

```bash
touch ~/.trusona
```

In this file, specify the email address you used to register in the Trusona App.

```bash
echo "user@example.com" > ~/.trusona
```

Restrict access to the file:

```bash
chmod 600 ~/.trusona
```

Some additional notes on user configuration:

* This file must be a regular file - not a symbolic link - and should have octal permissions of `10400` or `10600`.
* The file must be owned by the owner of `$HOME`.
* At runtime, `getuid()` must equal `geteuid()` as a precaution against any `setuid(uid_t)` [vulnerabilities](http://timetobleed.com/detailed-explanation-of-a-recent-privilege-escalation-bug-in-linux-cve-2010-3301/).
* Additionally, the contents of this file should be less than 128 bytes; otherwise, bytes beyond that count will be ignored.
* Finally, if this file is found, and its contents read, concatenation of the value specified by the `domain` setting will not be done.

### Testing the configuration

To test the configuration, attempt to send yourself a Trusonafication:

```bash

sudo trusona --user user@example.com

```

Where user@example.com is the email address you used to register in the Trusona App.

If the test fails, check the following:

1. Permissions of `/usr/local/etc/trusona/settings.json`
2. Validity of your API token and secret
3. Your email address is correct
4. Permissions and access of `~/.trusona`
5. Typos in `/usr/local/etc/trusona/settings.json`

## Enabling Trusona for SSH

Before completing the following steps, ensure that you have existing access to the machine where Trusona is being enabled for SSH. **This is critical in the event you need to revert any changes**

Restart the SSH service:

```bash

sudo service ssh restart

```


## How to Create a Debian/Ubuntu Package

On your Ubuntu Debian system, confirm that `make` completes successfully.

Install `checkinstall` with `[sudo] apt-get install checkinstall`

From inside the root of `trusona-server-sdk-c` run...

```bash
[sudo] debian/deb-builder.sh <semver version>
```

`<semver version>` should represent the version to build in a loosely enforced [Semver format](https://semver.org)

When it successfully completes, a `.deb` of the built package will be found in the current directory.
## How to Create a RPM Package

On your Ubuntu Debian system, confirm that `make` completes successfully.

Install `checkinstall` with `[sudo] apt-get install checkinstall`

From inside the root of `trusona-server-sdk-c` run...

```bash
[sudo] rpm/package-builder.sh <semver version>
```

`<semver version>` should represent the version to build in a loosely enforced [Semver format](https://semver.org)

When it successfully completes, a `.rpm` of the package will be found in the current directory.
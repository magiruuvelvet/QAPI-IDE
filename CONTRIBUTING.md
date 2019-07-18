# Contributing

Currently there are no contribution guidelines as this
project isn't in a working state. This file acts only
as some info for interested people.

## Commit Messages

Every commit message must start with a tag. Check the
commit log for real life examples.

## CI

### Operating Systems

This project is built and tested against the following
operating systems to check compilation and if the unit
tests are successful.

 - FreeBSD 12 with clang 8.0
 - Arch Linux with clang 8.0

Planned platforms for automated testing:

 - Windows (clang is preferred, MSVC compatibility is **not** guaranteed)
 - macOS (LLVM clang is preferred over Apple clang)

Transparent CI testing is a must when looking for CI services
or when setting up a new server from scratch. This means no GitHub
commit status changes (green check mark, red cross, etc.). If
someone known such a service please let me know.

### CI Skip

Commit messages matching the following regular expression
are not built to avoid wasting resources on my CI servers,
please make use of those when not changing actual code.

```
^readme\:.*$|^doc\:.*$|^tools\:.*$
```

This skips all commits which start with the tags

 - `readme:`
 - `doc:`
 - `tools:`


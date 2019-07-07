# QApi IDE

API Development and Testing Environment written in C++ and Qt

## Notice

This software is a draft right now. I attempt to build a native API development
testing tool, which share some of the ideas of Postman and Insomnia, but is
written in a real native programming language and not Electron and JS.

As an extend I also plan to keep this an 100% offline-only tool without phoning-home
spyware "features", no forced/broken auto-updates or constant telemetry pings. With
broken auto-updates I mean when the software is either installed globally using a
package manager or is running inside a restricted container with no write permissions.

Those useless and unwanted features bother me the most, which gave me enough motivation
to start my own API develoment tool. The closest thing for teams which this software
may ever see is a self-hosted server, as I have no interest in maintaining a centralized
"solution".

## Building

This project has git submodules. Don't forget to clone into them.

**Requirements**

The listed requirements below are recommendations. Older versions might work too, but are
not tested nor recommended and may contain unwanted bugs. If possible try to use recent
versions of said dependencies.

 - Qt 5.12+ (for the user interface only)
 - OpenSSL
 - zlib

Additional build requirements are:

 - C++17 compiler (clang 8+ or gcc 9+ are highly recommended)
 - cmake 3.12+

## Supported formats with pretty-printing (and syntax highlighting)

 - JSON
 - (maybe?) XML

## Unit Tests

Unit tests are built by default for now. Run them with the arguments `--reporter=info` for
a more detailed and nice output.

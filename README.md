# QApi IDE

API Development and Testing Environment written in C++ and Qt

| **Build Status**           |                 |
|----------------------------|-----------------|
| FreeBSD 12 / clang 8.0     | [![](https://jenkins.magiruuvelvet.gdn/job/QAPI-IDE/Platforms=freebsd/badge/icon?style=flat-square)](https://jenkins.magiruuvelvet.gdn/job/QAPI-IDE/Platforms=freebsd/lastBuild/console) |
| Arch Linux / clang 8.0     | [![](https://jenkins.magiruuvelvet.gdn/job/QAPI-IDE/Platforms=linux/badge/icon?style=flat-square)](https://jenkins.magiruuvelvet.gdn/job/QAPI-IDE/Platforms=linux/lastBuild/console) |
| Windows                    | no CI setup yet |
| macOS                      | no CI setup yet |


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

Scripting Interface dependencies:

 - **C++**: [cling](https://root.cern.ch/cling) built with `CMAKE_BUILD_TYPE=Release` and `BUILD_SHARED_LIBS=ON`\
   don't build a debug build or use static linkage, the link time increases heavily (minutes) and produces a
   gigabyte huge binary (2GB+) which is very slow and inefficient

 - **JavaScript**: no external dependencies, everything is bundled

Additional build requirements are:

 - C++17 compiler (LLVM clang 8+ is supported)
 - any C++17 runtime library is supported (libstdc++, libc++, ...)
 - cmake 3.12+

Unsupported compilers (you are on your own for now, good luck):

 - GNU/GCC (any version)
 - Microsoft MSVC (any version)
 - Apple clang (any version)

Those compilers are not supported for now in early development
to keep the code free from compiler macros. If you get any weird compile
errors or template errors don't report them to me.
Try LLVM clang version 8 or higher first to see if you get the same error,
before opening an issue. Thanks for understanding.

## Scripting Interface

I plan to support scripting similar to Postman. I want to support JavaScript and C++
as scripting backends. Scripting is disabled as of now by default in CMake.
Some minimal C++ and JavaScript running works already, but without binding of
variables and functions.

**Why C++**

Because why not and because I can. C++ is not enabled by default during CMake so
don't worry if you're not gonna use it. This is only for crazy people like me :)

## Supported formats with pretty-printing (and syntax highlighting)

 - JSON
 - (maybe?) XML

## Unit Tests

Unit tests are built by default for now. Run them with the arguments `--reporter=info` for
a more detailed and nice output.

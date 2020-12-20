# Gillespie simulation library

[![Build Status][build-badge]][build-url]
[![Boost License][license-badge]][license-url]
![C++14,17,20][cxx-badge]

Header-only Gillespie simulation library for C++14 and later.

- [Install](#install)
- [Examples](#examples)
- [Project Status](#project-status)
- [License](#license)

[build-badge]: https://github.com/snsinfu/gsp/workflows/build/badge.svg
[build-url]: https://github.com/snsinfu/gsp/actions?query=workflow%3Abuild
[license-badge]: https://img.shields.io/badge/license-Boost-blue.svg
[license-url]: ./LICENSE.txt
[cxx-badge]: https://img.shields.io/badge/C%2B%2B-14%2F17%2F20-orange.svg


## Install

- [Download gsp.hpp][header].

The library is zero dependency and single-header only. Just `#include` the
downloaded header file in your C++ code.

Alternatively, you can add [the hosted repository][repo] as a git submodule of
your repository and add the include directory to your include path.

[header]: https://raw.githubusercontent.com/snsinfu/gsp/master/include/gsp.hpp
[repo]: https://github.com/snsinfu/gsp.git


## Examples

Example codes are in the [examples](./examples) directory.

- [A-B-C chain reaction](./examples/abc.cc)


## Project Status

Very early. I made this library for quickly experimenting ideas in my hobbyist
studies on gene-regulatory networks.


## License

Boost Software License, Version 1.0.

# Lucena Utilities Library

The purpose of the Lucena Utilities Library (LUL) is to abstract away portions of the build environment when working with C++17 and later Standards. It’s a foundational tool that should allow code built on top of it to remain agnostic regarding the underlying compiler, Standard Library, and aspects of the runtime. Note that LUL is specifically not a build system, but it does try to shield a project from details of the build system at the code level.

## Getting Started

[CMake](https://cmake.org/) scripts are provided which will generate the library itself, test applications, and, later, documentation. We require at least CMake 3.11, so that will need to be [installed first](https://cmake.org/download/).

More detailed instructions are provided below, but for a quick start, simply clone the repository, open up a terminal window, change to the local repo directory, and execute the following:

Under macOS or Linux:

    > mkdir build
    > cd build
    > cmake ..
    > cmake --build . --config Release
    > sudo make install

Under Windows:

    > mkdir build
    > cd build
    > cmake -DCMAKE_INSTALL_PREFIX=/path/to/install ..
    > cmake --build . --config Release
    > cmake --build . --target install

This will generate a static library in the build directory, then copy it  to `/usr/local/lib` (or the specified Windows directory) and copy the `Lucena-Utilities` headers folder to `/usr/local/include` (or the specified Windows directory). Now link the library from your project, and reference it with the meta-header `<Lucena-Utilities/Lucena-Utilities.hpp>`. Check `<Lucena-Utilities/lulCompilerFlags.hpp>` and `<Lucena-Utilities/lulConfig.hpp>` for usage information. Additionally, in the `build/test` directory you’ll find test executables; these don’t get installed anywhere else.

Note that while there is a skeleton script to generate documentation, it doesn’t currently do anything. In the future, it will just draw from the very abundant comments in the public headers, so for now, look there for reference.

## Prerequisites

LUL requires a C++17 compiler with a mostly-conforming Standard Library; it provides optional tools to plug some holes in not-quite-there Standard Library implementations. It has been tested with Xcode 9.4 (and later versions), Microsoft Visual Studio 15.7 (and later versions) on Windows, and gcc 7.3 on Debian Buster. Experimental support is in for clang 6+ and gcc 8+. At this time, feature detection assumes that you’ll be using your compiler’s bundled Standard Library implementation; some additional future abstraction will mitigate this limitation.

## Building, Installing, and Testing

Currently, the project is transitioning to using CMake as the primary build system (at the same as the primary author is upping his CMake game). However, beyond basic functionality, **the CMake scripts are untested**. Originally, hand-built project files for a number of different IDEs were used, but they were dependent on a specific development environment and also didn’t lend themselves very well to automation. These project files are still available in the `proj` directory for reference, along with most of their support files, but they will be going away once the CMake transition is complete.

LUL is intended to be linked as a static library; it’s actually untested as a dynamic library, though most—if not all—of the required support is there, including appropriate symbol visibility and linker hints. While it would ideally be made available as a header-only library, at minimum, many of the Standard Library reference implementations it provides can’t ship that way. One of the remaining design goals is to finish refactoring it so that it’s possible to use as a header-only library if one is will to forgo using these reference implementations. Note that there is a `LUL_CONFIG_HEADERS_ONLY` CMake option, but it doesn’t actually doanything (yet).

The basic build instructions are provided under **Getting Started**.  We give two different methods since, by default, macOS and Linux installs are to `/usr/local`, which requires sudo (or root) privileges, while Windows has a different model. Ignoring these differences, we have the following, with line numbers added:

    1 > mkdir build
    2 > cd build
    3 > cmake -DCMAKE_INSTALL_PREFIX=/path/to/install ..
    4 > cmake --build . --config Release
    5 > cmake --build . --target install

Line 3 can be changed to:

    3 > cmake -G "<generator>" -DCMAKE_INSTALL_PREFIX=/path/to/install ..

Replacing `<generator>` with one of the supported [generator strings](https://cmake.org/cmake/help/v3.12/manual/cmake-generators.7.html), if, for example, you wanted to generate [Ninja build scripts](https://ninja-build.org/) or use some other non-default generator. The `-DCMAKE_INSTALL_PREFIX` switch could be left out if the default installation directory is acceptable.

A number of additional options are available; to see the full list, browse the root CMakeLists.txt file, use the CMake GUI, or run `cmake -L` in the build directory after executing line 3. These options can be added as additional `-D` switches after `-DCMAKE_INSTALL_PREFIX` on line 3. There are two major groupings:

 - LUL_CONFIG_IOS_TARGET
 - LUL_CONFIG_MACOS_TARGET
 - LUL_CONFIG_TVOS_TARGET
 - LUL_CONFIG_WATCHOS_TARGET

These are used by Apple platforms to differentiate between targets. Only one of these, at most, should be `ON`. If `CMAKE_OSX_DEPLOYMENT_TARGET` is Darwin, `LUL_CONFIG_MACOS_TARGET` will be `ON` by default if no other
options is set. Currently, CMake targets for watchOS and tvOS are broken; a future version of CMake will provide a better way of dealing with this, at which point these will be deprecated.

- LUL_CONFIG_FORCE_LOCAL_ANY
- LUL_CONFIG_FORCE_LOCAL_BIT_CAST
- LUL_CONFIG_FORCE_LOCAL_FILESYSTEM
- LUL_CONFIG_FORCE_LOCAL_OBSERVER_PTR
- LUL_CONFIG_FORCE_LOCAL_OPTIONAL
- LUL_CONFIG_FORCE_LOCAL_SPAN
- LUL_CONFIG_FORCE_LOCAL_VARIANT
- LUL_CONFIG_USE_BOOST_NETWORKING

These provide more nuanced control over Standard Library feature reference implementation selection. By default, all of these are `OFF`. The `FORCE_LOCAL` options all prevent the selection of native implementations (including `std::experimental` ones), by the header wrappers; generally speaking, these don’t ever need to be used. The `USE_BOOST` option is only available until we have an alternative; by default, the [Networking TS](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4734.pdf) is simply unavailable through lulNetworkingWrapper.hpp.

> Regarding the wrappers, note that they wrap *headers*, not *features*. As part of their function, they alias the features from the selected wrapped headers into a common namespace—`stdproxy`—using their `std` names. Since these are just type aliases (mostly), mangled names—such as those used in an ABI—will reflect their origins, meaning these are as brittle as anything else in C++ when it comes to providing external interfaces. Assume all the usual caveats regarding using `std` names in an API apply.

In line 4, the config may be chosen from the usual set with the usual meanings for CMake: Debug, Release, RelWithDebInfo and MinSizeRel.

Tests are generated by default and left in `<build>/test`. They can be automatically run by changing line 5 to:

    5 > cmake --build . --target test

Tests can be scripted in the usual way for CMake, for example in order to only install on success.

## To-Do

- update CMake min requirements once actual iOS support is available
- factor out CMake modules for wrangling compiler definitions specific to LUL (among other things)
- fix lulNetworkingWrapper so it actually _wraps_ something
- add lulCoroutineWrapper (wrapping std, std::experimental, and boost(?))
- add actual tests
- beef up the compile-time diagnostic script
- divide lulCompilerFlags into pieces devoted to platform, compiler, and library to reflect their (purported) independence
- factor i18n string conversion and other utilities into their own libraries or simple source distributions
- replace Status (and its dependents) with [std]::outcome and friends

## Contributing

Please read [CONTRIBUTING.md](./CONTRIBUTING.md) for details on the process for submitting pull requests to us.

Please note that this project is released with a Contributor Code of Conduct, documented in [CODE_OF_CONDUCT.md](./CODE_OF_CONDUCT.md) By participating in this project you agree to abide by its terms.

## Versioning

We use [SemVer](https://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/bitweeder/lul/tags).

## Authors

-   [**bitweeder**](https://bitweeder.com/) - _Originator and maintainer_

## License

This project is licensed under the University of Illinois/NCSA Open Source License - see the [LICENSE.md](./LICENSE.md) file for details.

Some portions of the project are governed by other, compatible licenses, as described in the licenses directory, notably:

- Reference implementations of `<any>`, `<filesystem>`, `<optional>`, `<span>`, and `<variant>`, as well as pertinent support code, are effectively taken verbatim from [libc++](https://libcxx.llvm.org/), and used under the University of Illinois/NCSA Open Source License. See [licenses/libc++ License.txt](licenses/libc%2B%2B%20License.txt) and [licenses/libc++ Credits.txt](licenses/libc%2B%2B%20Credits.txt).
- The `std::bitcast` reference implementation is adapted from [code written by JF Bastien](https://github.com/jfbastien/bit_cast/), used under the MIT license. See [licenses/bit_cast License.txt](./licenses/bit_cast%20License.txt).
- [Google Test](https://github.com/google/googletest) is used under the terms of the BSD 3-Clause license. See [licenses/bit_cast License.txt](./licenses/bit_cast%20License.txt).

## Acknowledgments

- **Billie Thompson** - [PurpleBooth](https://github.com/PurpleBooth) - _Thanks for the ([Readme template](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2))!_
- **[Coraline Ada Ehmke](http://where.coraline.codes/ "Coraline Ada Ehmke")** - _Creator of [The Contributor Covenant](https://www.contributor-covenant.org/), used here under the terms of the [Creative Commons Attribution License](https://github.com/ContributorCovenant/contributor_covenant/blob/master/LICENSE.md)_

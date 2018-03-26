# Catena C++

## Dependencies

### Library
* CMake

### Tests
* Google Test: https://github.com/google/googletest
* JsonCpp: https://github.com/open-source-parsers/jsoncpp

## Building the Library
The Catena C++ library is built with cmake.  To build the library run:

    $ cd catena-cpp
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

The compiled library is found at `lib/libcatena.a`

### Build Options
There are several options for the build process with cmake.  If you have any
questions on how to work with cmake, please refer to their documentation.

#### Tests
Build the library with tests (default = OFF) with the `CATENA_BUILD_TESTS`
option.  The tests use the Catena test vectors found at
https://github.com/medsec/catena-test-vectors.
Some of these tests use Catena variants with reduced garlic for a shorter
running time.  This doesn't reduce the code coverage.  The non-reduced tests can
be enabled with the `CATENA_FULL_TESTS` option.
The test vectors are included as a submodule.  To clone this repository with all
its submodules run

    $ git clone --recursive git@github.com:medsec/catena-cpp.git

#### Sanitizers
The Catena library can be built for clang's [address sanitizer][1] and
[memory sanitizer][2].  There are the `CATENA_SANITIZE_ADDRESS` and
`CATENA_SANITIZE_MEMORY` options to build with the respective sanitizer.
Note that it's impossible to use both at the same time an too build with a
sanitizer `CMAKE_CXX_COMPILER` must be set to `clang++`.

We provided a `sanitizecatena` binary for sanitizing the code by running every
functionality of Catena once.  To build this binary, use the `CATENA_SANITIZE`
option.

##### Example for using the Address Sanitizer

    $ cd catena-cpp
    $ mkdir build
    $ cd build
    $ cmake -DCATENA_SANITIZE=ON -DCATENA_SANITIZE_ADDRESS=ON \
      -DCMAKE_CXX_COMPILER="/usr/bin/clang++" ..
    $ make
    $ ./bin/sanitizecatena

##### Example for using the Memory Sanitizer

    $ cd catena-cpp
    $ mkdir build
    $ cd build
    $ cmake -DCATENA_SANITIZE=ON -DCATENA_SANITIZE_MEMORY=ON \
      -DCMAKE_CXX_COMPILER="/usr/bin/clang++" ..
    $ make
    $ ./bin/sanitizecatena

#### Big Endian Platforms
Build the library for big endian platforms with the `CATENA_BIG_ENDIAN_PLATFORM`
option.  This is currently untested.

#### WebAssembly
There are currently two options for WebAssembly:

`CATENA_SIMULATE_WASM` for simulated WASM memory management and
`CATENA_PLATFORM_WASM` to build for WASM.  At the moment none of these flags
work.

## Benchmarks
To build the benchmark binary use the `CATENA_BENCHMARKS` option.  The compiled
binary is found at `bin/benchmarks`.  Running the binary will create the
benchmark files.

## Contributing
When contributing to this project please ensure that the linter, the sanitizers
and the tests do not return any errors.

### Style Guide
The code tries to comply with [Google's C++ style guide][3].  We provided a
[config](CPPLINT.cfg) for [cpplint][4] and a [script][linter.sh] which runs
`cpplint` with the correct parameters for this project.

    $ ./linter.sh

## Troubleshooting

### Google Test
Since we have no affiliation with the Google Test library we can't give any real
support for it.  Nevertheless since we are nice people and encountered some
problems ourselves, we can give you the solutions we found for those.

#### Installing Google Test on Ubuntu
Installing Google Test with apt is not enough, since this installs no library
files.  We found this solution which worked for us:
https://askubuntu.com/questions/145887/why-no-library-files-installed-for-google-test

#### Installing Google Test on Debian
Installing Google Test on Debian works similar to Ubuntu.  For some reason the
built files aren't in the build root but in `googlemock/gtest`.  To install
Google Test on Debian run:

    $ sudo apt install libgtest-dev
    $ cd /usr/src/googletest
    $ sudo mkdir build
    $ cd build
    $ sudo cmake ..
    $ sudo make
    $ cd googlemock/gtest
    $ sudo cp libgtest* /usr/lib
    $ cd ../../..
    $ sudo rm -rf build

[1]: https://clang.llvm.org/docs/AddressSanitizer.html
[2]: https://clang.llvm.org/docs/MemorySanitizer.html
[3]: https://google.github.io/styleguide/cppguide.html
[4]: https://github.com/cpplint/cpplint

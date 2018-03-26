# please use a recent version of CMake
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project (Catena VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror -O3 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -Wall -Werror")
# set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
# set(CMAKE_CXX_LINK_FLAGS_COVERAGE "${CMAKE_CXX_LINK_FLAGS} -coverage")

option(CATENA_SANITIZE_ADDRESS "Detect memory errors" OFF)
option(CATENA_SANITIZE_MEMORY "Detect uninitialized reads" OFF)
# option(CATENA_COVERAGE "Compiles for generating code coverage report" OFF)

# if(CATENA_COVERAGE)
#   set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS_COVERAGE})
#   set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_COVERAGE})
# endif()

if(CATENA_SANITIZE_ADDRESS)
  find_program(CLANG_PATH clang++)
  if (NOT CLANG_PATH)
    message(FATAL_ERROR "clang++ not found")
  endif()
  if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "CMAKE_CXX_COMPILER must be clang++ for sanitizer")
  endif()
  set(CMAKE_BUILD_TYPE debug)

  set(CATENA_SANITIZE_ADDRESS_PARS "-fsanitize=address \
                                    -fno-omit-frame-pointer")

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
  ${CATENA_SANITIZE_ADDRESS_PARS} \
  -g \
  -O1")

  set(CMAKE_CXX_LINKER ${CLANG_PATH})
  set(CMAKE_CXX_LINK_FLAGS_DEBUG "${CMAKE_CXX_LINK_FLAGS_DEBUG} \
  ${CATENA_SANITIZE_ADDRESS_PARS}")
endif()

if(CATENA_SANITIZE_MEMORY)
  if(CATENA_SANITIZE_ADDRESS)
    message(FATAL_ERROR "It's impossible to use the address and memory \
      sanitizer at the same time.")
  endif()
  find_program(CLANG_PATH clang++)
  if (NOT CLANG_PATH)
    message(FATAL_ERROR "clang++ not found")
  endif()
  if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "CMAKE_CXX_COMPILER must be clang++ for sanitizer")
  endif()

  set(CMAKE_BUILD_TYPE debug)

  set(CATENA_SANITIZE_MEMORY_PARS "-fsanitize=memory \
                                   -fno-omit-frame-pointer")

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
  ${CATENA_SANITIZE_MEMORY_PARS} \
  -g \
  -O2")

  set(CMAKE_CXX_LINKER ${CLANG_PATH})
  set(CMAKE_CXX_LINK_FLAGS_DEBUG "${CMAKE_CXX_LINK_FLAGS_DEBUG} \
  ${CATENA_SANITIZE_MEMORY_PARS}")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()

# install libraries into correct locations on all platforms.
include(GNUInstallDirs)

option(CATENA_BENCHMARKS "Build Catena's benchmark binary" OFF)

add_subdirectory(catena)

if(CATENA_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

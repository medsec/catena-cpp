# please use a recent version of CMake
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project (Catena VERSION 0.2 LANGUAGES CXX)

option(CATENA_BUILD_TESTS "Build the tests" OFF)
option(CATENA_ALL_TESTS "Build the non-reduced tests" OFF)
option(CATENA_BIG_ENDIAN_PLATFORM "Build Catena for big endian platform" OFF)
option(CATENA_PLATFORM_WASM "Build for WebAssembly" OFF)
option(CATENA_SIMULATE_WASM "Simulate WebAssembly memory management" OFF)
option(CATENA_SANITIZE "Build the binary for sanitizing purposes" OFF)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/config.h.in"
  "${PROJECT_SOURCE_DIR}/include/config.h"
  )

add_library(catena STATIC
  src/catena.cpp
  src/config.cpp
  src/components/gamma/gamma_algorithm.cpp
  src/components/gamma/saltmix.cpp
  src/components/graph/bit_reversal_graph.cpp
  src/components/graph/double_butterfly_graph.cpp
  src/components/graph/drsample_graph.cpp
  src/components/graph/generic_graph.cpp
  src/components/graph/graph_algorithm.cpp
  src/hash/blake2b.cpp
  src/hash/blake2b_1.cpp
  src/hash/blamka.cpp
  src/hash/hash_algorithm.cpp
  src/helpers/misc.cpp
  src/helpers/uintx.cpp
  src/helpers/xorshift1024star.cpp
  # src/wasm-memory/address_chunk.cpp
  # src/wasm-memory/address.cpp
  # src/wasm-memory/address_store.cpp
  # src/wasm-memory/memory.cpp
  )

target_include_directories(catena PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS catena EXPORT CatenaConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory Put config file
# in per-project dir (name MUST match), can also just go into 'cmake'.
install(EXPORT CatenaConfig DESTINATION share/catena/cmake)

# This makes the project importable from the build directory
export(TARGETS catena FILE CatenaConfig.cmake)

if(CATENA_BUILD_TESTS)
  add_executable(testcatena
    test/test.cpp
    )

  enable_testing()
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})

  # check for jsoncpp version
  find_package(PkgConfig REQUIRED)

  pkg_check_modules(JSONCPP jsoncpp>=1.8)
  if(NOT JSONCPP_FOUND)
    pkg_check_modules(JSONCPP_OLD jsoncpp REQUIRED)
  endif()

  configure_file (
    "${PROJECT_SOURCE_DIR}/test/test.cpp.in"
    "${PROJECT_SOURCE_DIR}/test/test.cpp"
    )

  if(JSONCPP_OLD_FOUND)
    message(STATUS "Old version of JsonCpp found.\n\
    JsonCpp version number: ${JSONCPP_OLD_VERSION}")
    link_libraries(${JSONCPP_OLD_LIBRARIES})
    target_link_libraries(testcatena
      catena
      ${GTEST_BOTH_LIBRARIES}
      ${JSONCPP_OLD_LIBRARIES}
      )
  else()
    link_libraries(${JSONCPP_LIBRARIES})
    target_link_libraries(testcatena
      catena
      ${GTEST_BOTH_LIBRARIES}
      ${JSONCPP_LIBRARIES}
      )
  endif()
  add_test(runtests testcatena)
endif()

if(CATENA_SANITIZE)
  add_executable(sanitizecatena
    sanitize/main.cpp
    )
    target_link_libraries(sanitizecatena
      catena
      )
endif()

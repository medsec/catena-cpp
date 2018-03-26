# please use a recent version of CMake
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project (CatenaBenchmarks VERSION 1.0 LANGUAGES CXX)

add_executable(benchmarks
  main.cpp
  )

target_link_libraries(benchmarks
  PRIVATE catena
  )

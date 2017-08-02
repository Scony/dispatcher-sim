CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(simulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

add_library(simulator-library
  Fuzzy3.cpp
)

add_executable(simulator
  main.cpp
)

target_link_libraries(simulator simulator-library)

add_dependencies(simulator simulator-library)

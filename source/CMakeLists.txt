CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(simulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

add_executable(simulator
  main.cpp
)

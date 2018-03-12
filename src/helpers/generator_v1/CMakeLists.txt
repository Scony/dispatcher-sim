CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

add_executable(generator_v1
  main.cpp
)

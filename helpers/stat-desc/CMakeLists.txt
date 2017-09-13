CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(stat-desc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g")

add_executable(stat-desc
  main.cpp
)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(simulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g")

add_library(simulator-library
  Fuzzy3.cpp
  Input.cpp
  RandomDispatcher.cpp
  Simulator.cpp
  Cloud.cpp
  Solution.cpp
  DispatcherFactory.cpp
  SimpleDispatcher.cpp
  MaxDispatcher.cpp
  IDispatcher.cpp
  )

add_executable(simulator
  main.cpp
  )

target_link_libraries(simulator simulator-library)

add_dependencies(simulator simulator-library)

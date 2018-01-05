CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

find_package(GTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(SIMULATOR_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(${SIMULATOR_SOURCE_DIR})

link_directories(${SIMULATOR_SOURCE_DIR})

add_executable(simulator-test
  main.cpp
  Fuzzy3Tests.cpp
  ExecutionsSubjectAndListener.cpp
  SODispatcherTests.cpp
  LODispatcherTests.cpp
  LazyClairvoyantEstimatorTests.cpp
  ELODispatcherTests.cpp
  )

target_link_libraries(simulator-test
  simulator-library
  ${GTEST_BOTH_LIBRARIES}
  gmock_main
  gmock
  pthread
  )

add_test(NAME simulator-test COMMAND simulator-test)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose
  DEPENDS simulator-test)

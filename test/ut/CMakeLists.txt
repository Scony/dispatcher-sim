CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(SIMULATOR_SOURCE_DIR )

include_directories(
  ${CMAKE_SOURCE_DIR}/source
  ${CMAKE_SOURCE_DIR}/3rdparty/gtest
  )

add_executable(simulator-test
  $<TARGET_OBJECTS:gtest>
  Fuzzy3Tests.cpp
  ExecutionsSubjectAndListener.cpp
  SODispatcherTests.cpp
  LODispatcherTests.cpp
  LazyClairvoyantEstimatorTests.cpp
  ELODispatcherTests.cpp
  CloudTests.cpp
  SolutionTests.cpp
  MiscellaneousTests.cpp
  SJSODispatcherTests.cpp
  )

target_link_libraries(simulator-test
  simulator-library
  pthread
  )

add_dependencies(simulator
  simulator-library
  )

add_test(NAME simulator-test
  COMMAND simulator-test
  )

add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
  DEPENDS simulator-test
  )

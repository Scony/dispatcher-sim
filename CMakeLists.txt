CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(simulator)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++11 -O3 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${TARGET})
option(BUILD_TESTS "Include test targets" ON)

add_subdirectory(src)

IF(BUILD_TESTS)
  enable_testing()
  add_subdirectory(3rdparty EXCLUDE_FROM_ALL)
  add_subdirectory(test EXCLUDE_FROM_ALL)
ENDIF(BUILD_TESTS)

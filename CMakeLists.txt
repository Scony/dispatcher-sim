CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${TARGET})
option(BUILD_TESTS "Include test targets" ON)

add_subdirectory(source)
add_subdirectory(helpers/stat-desc)

IF(BUILD_TESTS)
  enable_testing()
  add_subdirectory(3rdparty EXCLUDE_FROM_ALL)
  add_subdirectory(test EXCLUDE_FROM_ALL)
ENDIF(BUILD_TESTS)

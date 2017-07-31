CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

enable_testing()

add_subdirectory(source)

IF(BUILD_TESTING)
add_subdirectory(test)
ENDIF(BUILD_TESTING)

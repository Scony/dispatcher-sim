file(GLOB ${PROJECT_NAME}-benchmarks-sources "*.cpp")

include_directories(
  ${PROJECT_SOURCE_DIR}/src/simulator
  ${PROJECT_SOURCE_DIR}/3rdparty/google-benchmark/src
  )

add_executable(${PROJECT_NAME}-benchmarks
  ${${PROJECT_NAME}-benchmarks-sources}
  )

target_link_libraries(${PROJECT_NAME}-benchmarks
  ${PROJECT_NAME}-library
  benchmark
  pthread
  )

add_dependencies(${PROJECT_NAME}-benchmarks
  ${PROJECT_NAME}-library
  benchmark
  )

add_custom_target(${PROJECT_NAME}-benchmarks-run
  COMMAND ./${PROJECT_NAME}-benchmarks
  DEPENDS ${PROJECT_NAME}-benchmarks
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB ${PROJECT_NAME}-ut-sources "*.cpp")

include_directories(
  ${PROJECT_SOURCE_DIR}/src/simulator
  ${PROJECT_SOURCE_DIR}/3rdparty/gtest
  )

add_executable(${PROJECT_NAME}-ut
  $<TARGET_OBJECTS:gtest>
  ${${PROJECT_NAME}-ut-sources}
  )

target_link_libraries(${PROJECT_NAME}-ut
  ${PROJECT_NAME}-library
  pthread
  )

add_dependencies(${PROJECT_NAME}-ut
  ${PROJECT_NAME}-library
  )

add_test(NAME ${PROJECT_NAME}-ut
  COMMAND ./${PROJECT_NAME}-ut
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
  )

add_custom_target(${PROJECT_NAME}-ut-run
  COMMAND ${CMAKE_CTEST_COMMAND} --verbose
  DEPENDS ${PROJECT_NAME}-ut
  )

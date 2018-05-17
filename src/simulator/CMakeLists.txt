CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

file(GLOB ${PROJECT_NAME}-library-sources "[A-Z]*.cpp")

add_library(${PROJECT_NAME}-library
  ${${PROJECT_NAME}-library-sources}
  )

add_executable(${PROJECT_NAME}
  main.cpp
  )

target_link_libraries(${PROJECT_NAME}
  ${PROJECT_NAME}-library
  )

add_dependencies(${PROJECT_NAME}
  ${PROJECT_NAME}-library
  )

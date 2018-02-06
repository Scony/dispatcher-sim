CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O3")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O3 -march=amdfam10 -mcx16 -mpopcnt")

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

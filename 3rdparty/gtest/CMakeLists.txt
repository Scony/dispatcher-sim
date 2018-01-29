include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers -Wno-unused-parameter")

add_library(gtest OBJECT
    gmock-gtest-all.cc
    gtest_main.cc
)

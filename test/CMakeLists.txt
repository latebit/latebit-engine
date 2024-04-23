cmake_minimum_required(VERSION 3.28.1)

# Project settings
project(test LANGUAGES C CXX)

# Create a test library to link against in all the tests
add_library(test ${TEST_LIB_FILES})
target_link_libraries(test "${TEST}/lib/*.cpp")
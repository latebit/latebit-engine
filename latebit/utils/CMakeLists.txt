cmake_minimum_required(VERSION 3.28.1)

# This is just a utility package which will be included in other packages
# It is not meant to be built on its own
project(utils VERSION ${VERSION} LANGUAGES C CXX)

if (WASM)
else()
  generate_tests("${PROJECT_NAME}")
endif()
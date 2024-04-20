cmake_minimum_required(VERSION 3.28.1)

# Project settings
project(utils VERSION ${VERSION} LANGUAGES C CXX)

add_package("${PROJECT_NAME}")

if (WASM)
  add_library(${PROJECT_NAME} ${utils_SRC})
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O3")
else()
  # Create shared library
  add_library(${PROJECT_NAME} SHARED ${utils_SRC})
  generate_tests("${PROJECT_NAME}")
endif()
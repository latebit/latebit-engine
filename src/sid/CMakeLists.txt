cmake_minimum_required(VERSION 3.28.1)

# Project settings
project(sid VERSION ${VERSION} LANGUAGES C CXX)
add_package(sid)
add_package(utils)

if (WASM)
  # Build the library
  add_library(sid ${sid_SRC})
  set_target_properties(sid PROPERTIES COMPILE_FLAGS "-O3")
else()
  add_library(sid STATIC ${sid_SRC} ${utils_SRC})
  generate_tests(sid)
endif()

# Install target (needed by CPack)
install(TARGETS sid DESTINATION lib)

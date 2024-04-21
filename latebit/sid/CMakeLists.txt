cmake_minimum_required(VERSION 3.28.1)

# Project settings
project(sid VERSION ${VERSION} LANGUAGES C CXX)
get_package_files(sid)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  # Build the library
  add_library(sid ${sid_SRC_FILES} $<TARGET_OBJECTS:utils-obj>)
  add_library(sid-obj OBJECT ${sid_SRC_FILES} $<TARGET_OBJECTS:utils-obj>)
  set_target_properties(sid PROPERTIES COMPILE_FLAGS "-O3")
else()
  add_library(sid STATIC ${sid_SRC_FILES} $<TARGET_OBJECTS:utils-obj>)
  add_library(sid-obj OBJECT ${sid_SRC_FILES} $<TARGET_OBJECTS:utils-obj>)
  generate_tests(sid)
endif()

# Install target (needed by CPack)
install(TARGETS sid DESTINATION lib)

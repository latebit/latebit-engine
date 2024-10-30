cmake_minimum_required(VERSION 3.28.1)

project(ui VERSION ${VERSION} LANGUAGES C CXX)
get_package_files(ui)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  add_library(ui ${ui_SRC_FILES})
  target_link_libraries(ui latebit)
else()
  add_library(ui SHARED ${ui_SRC_FILES})
  target_link_libraries(ui latebit)

  generate_tests("${PROJECT_NAME}")
endif()

install(TARGETS ui DESTINATION lib)
cmake_minimum_required(VERSION 3.28.1)

project(integration_test LANGUAGES C CXX)

file(GLOB_RECURSE INTEGRATION_TESTS_FILES "${CMAKE_SOURCE_DIR}/test/integration/*")

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
foreach(test_file ${INTEGRATION_TESTS_FILES})
  get_filename_component(FILE_NAME ${test_file} NAME_WE)
  add_executable(${FILE_NAME}.test ${test_file})
  set_target_properties(${FILE_NAME}.test PROPERTIES LINK_FLAGS "-s USE_LIBPNG=1 -s USE_SDL=2 -sSTACK_SIZE=1MB --preload-file ../../test/fixtures")
  set_target_properties(${FILE_NAME}.test PROPERTIES SUFFIX ".html")
  target_link_libraries(${FILE_NAME}.test PRIVATE latebit latebit-test ui)
endforeach()
else ()
foreach(test_file ${INTEGRATION_TESTS_FILES})
  get_filename_component(FILE_NAME ${test_file} NAME_WE)
  add_executable(${FILE_NAME}.test ${test_file})
  target_link_libraries(${FILE_NAME}.test latebit latebit-test ui)
  # Note: not adding the test to ctest, as these tests are meant to be run manually
endforeach()
endif()
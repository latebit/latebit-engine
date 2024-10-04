enable_testing()
set(TEST ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_LIB_FILES "${TEST}/lib/*.cpp")
add_library(latebit-test ${TEST_LIB_FILES})
target_link_libraries(latebit-test "latebit")

# Generate test executable
function(generate_tests PACKAGE_NAME)
  foreach(test_file ${${PACKAGE_NAME}_TEST_FILES})
    get_filename_component(DIRECTORY ${test_file} DIRECTORY)
    get_filename_component(FILE_NAME ${test_file} NAME_WE)
    file(RELATIVE_PATH RELATIVE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${DIRECTORY})
    add_executable(${FILE_NAME}.test ${test_file})
    target_link_libraries(${FILE_NAME}.test latebit latebit-test)

    set(TEST_NAME "${PACKAGE_NAME}/${RELATIVE_DIRECTORY}/${FILE_NAME}")
    string(REPLACE "//" "/" TEST_NAME "/${TEST_NAME}")  # Ensure no double slashes

    add_test(NAME "${TEST_NAME}" COMMAND ${FILE_NAME}.test)
  endforeach()
endfunction()
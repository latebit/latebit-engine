cmake_minimum_required(VERSION 3.28.1)

if(DEFINED ENV{VERSION})
  set(VERSION $ENV{VERSION})
else()
  set(VERSION "0.0.0")
endif()

option(WASM "Build for WebAssembly" OFF)

# Compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wpedantic -gdwarf-2 -fsanitize=address -fno-omit-frame-pointer")

# Project settings
project(latebit VERSION ${VERSION} LANGUAGES C CXX)

# Project files
set(SRC ${CMAKE_SOURCE_DIR}/src)
file(GLOB SRC_FILES "${SRC}/core/**/*.cpp" "${SRC}/core/*.cpp" "${SRC}/sid/synth/utils.cpp" "${SRC}/sid/synth/tune.cpp" "${SRC}/sid/synth/track.cpp" "${SRC}/sid/synth/oscillator.cpp")
list(FILTER SRC_FILES EXCLUDE REGEX ".test.c$")
file(GLOB INCLUDE_FILES "${SRC}/core/**/*.h" "${SRC}/core/*.h" "${SRC}/sid/synth/utils.h" "${SRC}/sid/synth/tune.h" "${SRC}/sid/synth/track.h" "${SRC}/sid/synth/oscillator.h")

set(TEST ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_LIB_FILES "${TEST}/lib/*.cpp")
file(GLOB TEST_SUITE_FILES "${TEST}/suites/*.cpp")

enable_testing()
file(GLOB TEST_FILES "${SRC}/sid/synth/*.test.cpp")
foreach(test_file ${TEST_FILES})
  get_filename_component(test_name ${test_file} NAME_WE)
  add_executable(${test_name} ${test_file} ${TEST_LIB_FILES})
  target_link_libraries(${test_name} ${PROJECT_NAME})
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()


set(ALL_FILES ${SRC_FILES} ${TEST_LIB_FILES} ${TEST_SUITE_FILES} ${INCLUDE_FILES})

# This allows clean includes (such as "core/graphics/..." instead of "../src/core/...")
include_directories(${SRC})

if (WASM)
    include(cmake/WebAssembly.cmake)
else()
    include(cmake/Native.cmake)
endif()

add_custom_target(format
	COMMAND clang-format -n --Werror -ferror-limit=1 ${ALL_FILES}
)

add_custom_target(tidy
    COMMAND clang-tidy 
            --fix
            --fix-errors
            --fix-notes
            --warnings-as-errors=*
            --header-filter='${INCLUDE}/.*'
            ${ALL_FILES}
            --
            ${CMAKE_CXX_FLAGS}
            -I${INCLUDE}
            -I${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
)

include(cmake/Release.cmake)
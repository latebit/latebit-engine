cmake_minimum_required(VERSION 3.18)

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

# Project settings
project(latebit VERSION ${VERSION} LANGUAGES CXX)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDE})
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB TEST_LIB_FILES "test/lib/*.cpp")
file(GLOB TEST_SUITE_FILES "test/suites/*.cpp")
file(GLOB INCLUDE_FILES "${INCLUDE}/*.h")

if (WASM)
    include(cmake/WebAssembly.cmake)
else()
    include(cmake/Native.cmake)
endif()

add_custom_target(format
	COMMAND clang-format -n --Werror -ferror-limit=1 ${SRC_FILES} ${TEST_LIB_FILES} ${TEST_SUITE_FILES} ${INCLUDE_FILES}
)

add_custom_target(tidy
    COMMAND clang-tidy --fix --fix-errors --fix-notes --warnings-as-errors=* --header-filter='${INCLUDE}/.*' ${SRC_FILES} ${TEST_LIB_FILES} ${TEST_SUITE_FILES} -- ${CMAKE_CXX_FLAGS} -I${INCLUDE} -I${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
)

include(cmake/Release.cmake)
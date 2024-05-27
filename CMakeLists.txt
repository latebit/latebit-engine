cmake_minimum_required(VERSION 3.28.1)
project(latebit)

include(cmake/Config.cmake)
include(cmake/Testing.cmake)
include(cmake/Utils.cmake)
include(cmake/Package.cmake)

set(SRC ${CMAKE_SOURCE_DIR}/latebit)
file(GLOB_RECURSE ALL_FILES "${SRC}/*")

# This allows clean includes (such as "core/graphics/..." instead of "../src/core/...")
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(latebit/utils)
add_subdirectory(latebit/sid)
add_subdirectory(latebit/core)
add_subdirectory(test)

add_custom_target(format
	COMMAND clang-format -n --Werror -ferror-limit=1 ${ALL_FILES}
)

add_custom_target(tidy
    COMMAND clang-tidy 
            --fix
            --fix-errors
            --fix-notes
            --warnings-as-errors=*
            --header-filter=.*
            ${ALL_FILES}
            --
            ${CMAKE_CXX_FLAGS}
            -I${SRC}
            -I${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
)
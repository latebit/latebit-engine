cmake_minimum_required(VERSION 3.18)

# Enable vcpkg
set(VCPKG_ROOT $ENV{VCPKG_ROOT})
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Compiler settings
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Project settings
project(latebit VERSION 0.1.0 LANGUAGES C CXX)

# Declare dependencies
find_package(SDL2 2.30.0 CONFIG REQUIRED)
find_package(SDL2_ttf 2.22.0 CONFIG REQUIRED)
find_package(SDL2_mixer 2.8.0 CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB TEST_LIB_FILES "test/lib/*.cpp")

# Creates the library
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    SDL2::SDL2-static
    SDL2_mixer::SDL2_mixer-static
    SDL2_ttf::SDL2_ttf-static)
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})

# Creates the test executable
add_executable(test test/main.cpp ${TEST_LIB_FILES})
target_link_libraries(test PRIVATE ${PROJECT_NAME})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "DEB;TGZ;DragNDrop")
set(CPACK_PACKAGE_CONTACT "manuelspagnolo@duck.com")
include(CPack)
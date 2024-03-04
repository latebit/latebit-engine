set(VCPKG_ROOT $ENV{VCPKG_ROOT})
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_CXX_COMPILER "clang++")

# Declare dependencies
find_package(SDL2 2.30.0 CONFIG REQUIRED)
find_package(SDL2_ttf 2.22.0 CONFIG REQUIRED)
find_package(SDL2_mixer 2.8.0 CONFIG REQUIRED)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    SDL2::SDL2-static
    SDL2_mixer::SDL2_mixer-static
    SDL2_ttf::SDL2_ttf-static)
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})

# Create test executable
add_executable(test test/main.cpp ${TEST_LIB_FILES})
target_link_libraries(test PRIVATE ${PROJECT_NAME})

# Install target (sed by CPack)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${SDL2_INCLUDE_DIR} DESTINATION include)

# Package with CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "DEB;TGZ")
set(CPACK_PACKAGE_CONTACT "manuelspagnolo@duck.com")
set(CPACK_PACKAGE_DESCRIPTION "A pixel art game engine integrated in your IDE.")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Manuel Spagnolo")
include(CPack)
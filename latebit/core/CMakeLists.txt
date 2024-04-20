cmake_minimum_required(VERSION 3.28.1)

# Project settings
project(latebit VERSION ${VERSION} LANGUAGES C CXX)
add_package(core)
add_package(utils)

if (WASM)
  # Build the library
  add_library(latebit ${core_SRC})
  set_target_properties(latebit PROPERTIES COMPILE_FLAGS "-s USE_LIBPNG=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -O3")
else()
  # Declare dependencies
  find_package(SDL2 CONFIG REQUIRED)
  find_package(SDL2_mixer CONFIG REQUIRED)
  find_package(PNG REQUIRED)

  # Create shared library
  add_library(latebit SHARED ${core_SRC} ${utils_SRC})
  target_link_libraries(latebit 
      PUBLIC
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      sid
      PNG::PNG
      SDL2::SDL2-static
      SDL2_mixer::SDL2_mixer-static)

  generate_tests(core)

  # TODO: for some reason SDL2_INCLUDE_DIR is wrong in CI, we override it manually before packaging
  set(SDL2_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include/SDL2")
endif()


# Install target (needed by CPack)
install(TARGETS latebit DESTINATION lib)
install(DIRECTORY ${SDL2_INCLUDE_DIR} DESTINATION include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/latebit/ DESTINATION include/latebit
        FILES_MATCHING PATTERN "*.h")
set(CPACK_PACKAGE_DESCRIPTION "A game engine integrated in your IDE for pixel art games.")

include(CPack)
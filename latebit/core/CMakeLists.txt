cmake_minimum_required(VERSION 3.28.1)

# Project settings
project(latebit VERSION ${VERSION} LANGUAGES C CXX)
get_package_files(core)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  # Build the library
  add_library(latebit ${core_SRC_FILES} $<TARGET_OBJECTS:utils-obj> $<TARGET_OBJECTS:sid-obj>)
  set_target_properties(latebit PROPERTIES COMPILE_FLAGS "-s USE_LIBPNG=1 -s USE_SDL=2 -O3")
  target_link_libraries(latebit PUBLIC sid)
else()
  # Declare dependencies
  find_package(SDL2 CONFIG REQUIRED)
  find_package(PNG REQUIRED)

  # Create shared library
  add_library(latebit SHARED ${core_SRC_FILES} $<TARGET_OBJECTS:utils-obj> $<TARGET_OBJECTS:sid-obj>)
  target_link_libraries(latebit 
      PUBLIC
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      sid
      PNG::PNG
      SDL2::SDL2-static)

  generate_tests(core)
  # TODO: for some reason SDL2_INCLUDE_DIR is wrong in CI, we override it manually before packaging
  set(SDL2_INCLUDE_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include/SDL2")
endif()

# Install target (needed by CPack)
install(TARGETS latebit DESTINATION lib)
install(DIRECTORY ${SDL2_INCLUDE_DIR} DESTINATION include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/latebit/ DESTINATION include/latebit
        FILES_MATCHING PATTERN "*.h")

include(CPack)

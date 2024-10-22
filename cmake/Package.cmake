# Shared CPACK configuration for all the packages
set(CPACK_PACKAGE_VENDOR "Manuel Spagnolo")
set(CPACK_PACKAGE_CONTACT "manuelspagnolo@duck.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A 2D game engine integrated in your IDE")
set(CPACK_PACKAGE_VERSION ${VERSION})

execute_process(
  COMMAND ${CMAKE_C_COMPILER} -dumpmachine
  RESULT_VARIABLE RESULT
  OUTPUT_VARIABLE ARCH
  ERROR_QUIET
)
if (RESULT)
  message(FATAL_ERROR "Failed to determine target architecture triplet: ${RESULT}")
endif()
string(REGEX MATCH "([^-]+).*" ARCH_MATCH ${ARCH})
if (NOT CMAKE_MATCH_1 OR NOT ARCH_MATCH)
  message(FATAL_ERROR "Failed to match the target architecture triplet: ${ARCH}")
endif()
set(ARCH ${CMAKE_MATCH_1})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${VERSION}-${ARCH}-Linux")
  set(CPACK_GENERATOR "DEB;TGZ")
elseif(APPLE)
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${VERSION}-${CMAKE_OSX_ARCHITECTURES}-Darwin")
  set(CPACK_GENERATOR "DragNDrop;TGZ")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${VERSION}-wasm")
  set(CPACK_GENERATOR "TGZ")
else()
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${VERSION}-${ARCH}-${CMAKE_SYSTEM_NAME}")
  set(CPACK_GENERATOR "TGZ")
endif()
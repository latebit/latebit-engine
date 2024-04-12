# Install target (needed by CPack)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${SDL2_INCLUDE_DIR} DESTINATION include)

message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIRS}")

# Package with CPack
set(CPACK_PACKAGE_VENDOR "Manuel Spagnolo")
set(CPACK_PACKAGE_CONTACT "manuelspagnolo@duck.com")
set(CPACK_PACKAGE_DESCRIPTION "A game engine integrated in your IDE for pixel art games.")
set(CPACK_PACKAGE_VERSION ${VERSION})

if (WASM)
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION_TAG}-wasm")
else()
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION_TAG}-${CMAKE_SYSTEM_NAME}")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(CPACK_GENERATOR "DEB;TGZ")
elseif (APPLE)
  set(CPACK_GENERATOR "DragNDrop;TGZ")
endif()

include(CPack)
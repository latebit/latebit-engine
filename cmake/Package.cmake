# Shared CPACK configuration for all the packages
set(CPACK_PACKAGE_VENDOR "Manuel Spagnolo")
set(CPACK_PACKAGE_CONTACT "manuelspagnolo@duck.com")
set(CPACK_PACKAGE_VERSION ${VERSION})

if (WASM)
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${VERSION}-wasm")
  set(CPACK_GENERATOR "TGZ")
else()
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${VERSION}-${CMAKE_SYSTEM_NAME}")
  if (APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ")
  else()
    set(CPACK_GENERATOR "DEB;TGZ")
  endif()
endif()
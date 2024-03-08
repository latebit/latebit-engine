# Install target (needed by CPack)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${SDL2_INCLUDE_DIR} DESTINATION include)

# Package with CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Manuel Spagnolo")
set(CPACK_PACKAGE_CONTACT "manuelspagnolo@duck.com")
set(CPACK_PACKAGE_DESCRIPTION "A pixel art game engine integrated in your IDE.")

include(CPack)
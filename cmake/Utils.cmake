# Create a function to easily add a package to the build
function(add_package PACKAGE_NAME)
  file(GLOB PACKAGE_SRC "${SRC}/${PACKAGE_NAME}/**/*.cpp" "${SRC}/${PACKAGE_NAME}/*.cpp")
  list(FILTER PACKAGE_SRC EXCLUDE REGEX ".test.cpp$")
  file(GLOB PACKAGE_TEST "${SRC}/${PACKAGE_NAME}/**/*.test.cpp" "${SRC}/${PACKAGE_NAME}/*.test.cpp")
  file(GLOB PACKAGE_HEADERS "${SRC}/${PACKAGE_NAME}/**/*.h" "${SRC}/${PACKAGE_NAME}/*.h")

  set("${PACKAGE_NAME}_SRC" ${PACKAGE_SRC} PARENT_SCOPE)
  set("${PACKAGE_NAME}_HEADERS" ${PACKAGE_HEADERS} PARENT_SCOPE)
  set("${PACKAGE_NAME}_TEST_FILES" ${PACKAGE_TEST} PARENT_SCOPE)
endfunction()

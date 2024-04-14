# Declare dependencies
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(PNG REQUIRED)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    PNG::PNG
    SDL2::SDL2-static
    SDL2_mixer::SDL2_mixer-static)

# Create test executable
add_executable(test.out test/main.cpp ${TEST_LIB_FILES})
target_link_libraries(test.out PRIVATE ${PROJECT_NAME})
add_custom_target(test
    COMMAND LSAN_OPTIONS=suppressions=asan.supp $<TARGET_FILE:test.out>
    DEPENDS test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
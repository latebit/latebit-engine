set(CMAKE_CXX_COMPILER "emcc")
set(CMAKE_AR "emar")
set(CMAKE_RANLIB ":")

# Build the library
add_library(${PROJECT_NAME} ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-s USE_SDL=2 -s USE_SDL_MIXER=2 -O3")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL_mixer)
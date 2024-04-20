if(DEFINED ENV{VERSION})
  set(VERSION $ENV{VERSION})
else()
  set(VERSION "0.0.0")
endif()

option(WASM "Build for WebAssembly" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wpedantic -gdwarf-2 -fsanitize=address -fno-omit-frame-pointer")
set(SRC ${CMAKE_SOURCE_DIR}/src)

if(WASM)
  set(CMAKE_CXX_COMPILER "emcc")
  set(CMAKE_AR "emar")
  set(CMAKE_RANLIB ":")
endif()
if(DEFINED ENV{VERSION})
  set(VERSION $ENV{VERSION})
else()
  set(VERSION "0.0.0")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -gdwarf-2 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer")

enable_testing()
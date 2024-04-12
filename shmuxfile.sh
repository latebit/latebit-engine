wasm:
  # Creates a wasm build
  emmake cmake -DWASM=1 -B build-wasm .
  emmake cmake --build build-wasm

native:
  # Creates a native build
  cmake --toolchain=~/vcpkg/scripts/buildsystems/vcpkg.cmake -B build .
  cmake --build build -j 4

debug:
  # Creates a debug build
  cmake --toolchain=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -B build .
  cmake --build build

tidy:
  # Lints the code with clang-tidy
  cmake --build build -t tidy

format:
  # Formats the code with clang-format
  cmake --build build -t format

package:
  # Builds the package with cpack
  cmake --build build -t package

test:
  # Runs the test executable
  cmake --build build -t test

prepare: clean
  # Prepares current directory for development
  cmake --toolchain=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build .
  ln -s build/compile_commands.json compile_commands.json

clean:
  # Cleans the directory from build artifacts
  rm -rf build build-wasm compile_commands.json
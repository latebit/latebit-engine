wasm:
  # Creates a wasm build
  emmake cmake -DWASM=1 -B build-wasm .
  emmake cmake --build build-wasm

native:
  # Creates a native build
  cmake --toolchain=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release -B build .
  cmake --build build -j8

debug:
  # Creates a debug build
  export LSAN_OPTIONS="suppressions=asan.supp"
  cmake --toolchain=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -B build .
  cmake --build build -j8

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
  cmake --build build -j8 -t test

prepare: clean
  # Prepares current directory for development
  ln -s build/compile_commands.json compile_commands.json

clean:
  # Cleans the directory from build artifacts
  rm -rf build build-wasm compile_commands.json

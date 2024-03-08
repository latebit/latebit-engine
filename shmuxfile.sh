wasm:
  # Creates a wasm build
  export CXX=clang++-17
  emmake cmake -DWASM=1 -B build-wasm .
  emmake cmake --build build-wasm

native:
  # Creates a native build
  export CXX=clang++-17
  cmake --toolchain=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -B build .
  cmake --build build

tidy: native
  # Lints the code with clang-tidy
  cmake --build build -t tidy --verbose

package: native
  # Builds the package with cpack
  cmake --build build -t package

test: native
  # Runs the test executable
  ./build/test

debug: native
  # Runs gdb on the test executable for debugging
  gdb ./build/test

memory: native
  # Runs valgrind on the test executable to check for memory leaks
  valgrind --leak-check=full ./build/test

prepare: clean
  # Prepares current directory for development
  export CXX=clang++-17
  cmake --toolchain=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build .
  ln -s build/compile_commands.json compile_commands.json

clean:
  # Cleans the directory from build artifacts
  rm -rf build build-wasm compile_commands.json
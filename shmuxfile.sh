wasm:
  emmake cmake -DWASM=1 -B build-wasm .
  emmake cmake --build build-wasm

native:
  cmake --toolchain=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -B build .
  cmake --build build

package: native
  cd build
  cpack

test: native
  ./build/test

debug: native
  gdb ./build/test

memory: native
  valgrind --leak-check=full ./build/test

clean:
  rm -rf build build-wasm
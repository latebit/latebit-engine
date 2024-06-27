Testing
===

# Integration testing

Integration tests are test that are **not** run in CI, but rather manually by 
the developers during development and review. 

They are used in the following two cases:

 * when a unit test is impactical or impossible 
   for example, when a behaviour depends heavily on the hardware or SDL

 * when you want to see something happening in a real game, but you cannot

They are currently run both in native environments and in WASM.

## Running the tests

### Native

```sh
# Normal configure and build
cmake -B build && cmake --build build 

# Launch the test
./build/test/NAME_OF_THE_TEST.test
```

### WASM

```sh
# Normal configure and build
emcmake cmake -B build && emmake cmake --build build 

# Launch the test
emrun ./build/test/NAME_OF_THE_TEST.test.html
```
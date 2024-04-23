latebit
===

A game engine integrated in your IDE for pixel art games.

## :warning: Warning

This is very much in progress and far from stable. Use it a your own risk.

## Development

### Dependencies

The dependencies required to run the project locally are:
  
  * [LLVM/Clang](https://releases.llvm.org/): `>=15.0.6`
  * [cmake](https://cmake.org/download/): `>=3.28.1`
  * [vcpkg](https://github.com/microsoft/vcpkg): `>=2024-02-07-8a83681f921b10d86ae626fd833c253f4f8c355b`

While not a strict dependency, [shmux](https://github.com/shikaan/shmux) may be convenient to save some typing.

Once your system is set, you should be able to run

```sh
# if you have shmux installed
shmux prepare

# otherwise
cmake --toolchain=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build .
ln -s build/compile_commands.json compile_commands.json
```

At this point you should be good to go. If your editor makes use of `clangd`, you should also see autocompletion and formatting hints.

### Running tests

```sh
# Run all the tests
shmux test # i.e. `cmake --build build -t test`

# Run headless
SDL_AUDIODRIVER=dummy SDL_VIDEODRIVER=dummy shmux test
```

### Debugging the test executable

Usual development tools are available and can be used against the test executable

```sh
# Create a test executable with debug symbols
shmux debug

# To debug
gdb ./build/test.out
```

Debug builds are instrumented with Address Sanitizer, which should help with memory leaks detection.

### Linting and formatting code

It's highly encouraged that you set your IDE to run these checks while you code, but
should you need it, you can also run format and tidy manually.

```sh
shmux format # i.e. `cmake --build build -t format`

shmux tidy # i.e. `cmake --build build -t tidy`
```

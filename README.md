LateBit
===

A game engine integrated in your IDE for pixel art games.

## :warning: Warning

This is very much in progress and far from stable. Use it a your own risk.

## Development

### Dependencies

The quickest way to setup your local environment to develop latebit is using [setup-cpp](https://github.com/aminya/setup-cpp). This is the same tool used CI and will guarantee the same results locally and in CI.

The dependencies we need in particular are:
  
  * compiler: `llvm-15.0.3`
  * cmake: `3.28.1`
  * vcpkg: `true`
  * clangtidy: `true` 

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

# Run only a suite (e.g., Sprite)
FOCUS=Sprite shmux test # i.e. `FOCUS=Sprite cmake --build build -t test`

# Show only failed tests
FAILED_ONLY=1 shmux test # i.e. `FOCUS=Sprite cmake --build build -t test`

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

# To check memory
valgrind --leak-check=full ./build/test.out
```

### Linting and formatting code

It's highly encouraged that you set your IDE to run these checks while you code, but
should you need it, you can also run format and tidy manually.

```sh
shmux format # i.e. `cmake --build build -t format`

shmux tidy # i.e. `cmake --build build -t tidy`
```
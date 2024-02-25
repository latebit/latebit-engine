LateBits
===

A minimal game engine integrated in your IDE to create pixel based games.

## :warning: Warning

This is very much in progress and far from stable. Use it a your own risk.

## Dependencies

This engine relieas heavily on SDL for multimedia and input handling. 
In order to build and run this project, you would need to install [SDL](https://wiki.libsdl.org/SDL2/FrontPage), [SDL_ttf](https://wiki.libsdl.org/SDL2_ttf/FrontPage), and [SDL_mixer](https://wiki.libsdl.org/SDL2_mixer/FrontPage).


## Running tests

```sh
# Run all the tests on a production build
make test

# Run all the tests with debug symbols
DEBUG=1 make test

# Run only a suite (e.g., Sprite)
FOCUS=Sprite make test

# Show only failed tests
FAILED_ONLY=1 make test
```

## Debugging (by running the tests)

```
make debug
```

## Format code to abide to current standards

```
make format tidy
```
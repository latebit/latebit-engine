#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include <cstdint>

#include "Colors.h"
#include "Font.h"
#include "latebit/core/geometry/Box.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Keyframe.h"

using namespace std;

#define DM lb::DisplayManager

namespace lb {

namespace TextAlignment {
enum TextAlignment {
  LEFT,
  CENTER,
  RIGHT,
};
}

namespace TextSize {
enum TextSize {
  NORMAL = 1,
  LARGE,
  XLARGE,
  XXLARGE,
};
}

// A 2D Vector representing a position in the world in cells
using Position = Vector;

const Color::Color WINDOW_BACKGROUND_COLOR_DEFAULT = Color::BLACK;

const string DUMMY_VIDEODRIVER = "dummy";

// Converts cell coodinates to pixel coordinates
auto cellsToPixels(Position cells) -> Vector;
// Converts pixel coodinates to cell coordinates
auto pixelsToCells(Vector pixels) -> Position;

class DisplayManager {
 private:
  DisplayManager() = default;
  // The window has an initial given size in pixels and cells. All we draw
  // are characters and all the coordinates we use are in cells
  static SDL_Window *window;
  static SDL_Renderer *renderer;
  // Background color of the window
  static Color::Color backgroundColor;

  // Returns appropriate renderer flags for the current driver
  [[nodiscard]] static auto getRendererFlags() -> int;

 public:
  // Window width in cells
  static constexpr int WINDOW_WIDTH = 240;
  // Window width in cells
  static constexpr int WINDOW_HEIGHT = 160;

  [[nodiscard]] static auto isStarted() -> bool;

  // Sets up the window and font
  static auto startUp() -> int;
  static void shutDown();

  // Draws a frame in the given position, scaling it by the given factor
  [[nodiscard]] static auto drawKeyframe(Position position,
                                         const Keyframe *frame, uint8_t width,
                                         uint8_t height, uint8_t scaling)
    -> int;

  // Draws a rectangle outline at the given world position (top left cell)
  [[nodiscard]] static auto drawRectangle(Position position, int width,
                                          int height, Color::Color borderColor)
    -> int;

  // Draws a rectangle at the given world position (top left cell)
  [[nodiscard]] static auto drawRectangle(Position position, int width,
                                          int height, Color::Color borderColor,
                                          Color::Color fillColor) -> int;

  // Draws a string to the window at the given world position
  [[nodiscard]] static auto drawString(
    Position postion, string string, TextAlignment::TextAlignment alignment,
    Color::Color color, TextSize::TextSize size = TextSize::NORMAL,
    Font font = DEFAULT_FONT) -> int;

  // Returns the bounding box of a given string. Dimensions are in cells
  [[nodiscard]] static auto measureString(
    string string, TextSize::TextSize size = TextSize::NORMAL,
    Font font = DEFAULT_FONT) -> Box;

  // Change the background color of the window
  static void setBackground(Color::Color color);

  // Swap the buffers for drawing.
  // This is the result of Double Buffering: first we draw to a hidden buffer
  // and then we display it to the screen
  static auto swapBuffers() -> int;
};
}  // namespace lb

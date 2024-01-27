#pragma once

#include <string>
#include <vector>

#include "Colors.h"
#include "Frame.h"
#include "Vector.h"

using namespace std;

namespace df {
enum Slowdown {
  STOANIMATION = 0,
  NO_SLOWDOWN = 1,
};

class Sprite {
 private:
  // The width of the sprite
  int width = 0;
  // The height of the sprite
  int height = 0;
  // The maximum number of frames the sprite can hold
  int maxFrameCount = 0;
  // The number of frames the sprite currently holds
  int frameCount = 0;
  // The number of frames to wait before changing frame
  int slowdown = NO_SLOWDOWN;
  // The color of the sprite
  Color color = COLOR_DEFAULT;
  // The frames of the sprite
  vector<Frame> frames = {};
  // The label of the sprite, used to reference it
  string label = "";
  // The character to use for transparency (i.e., not drawn)
  char transparencyChar = ' ';

 public:
  ~Sprite();
  // Creates a sprite with a given maximum number of frames
  Sprite(int maxFrames = 0);
  // Creates a sprite from another sprite
  Sprite(const Sprite& other);
  // Checks if two sprites are equal
  auto operator==(const Sprite& other) const -> bool;

  // Set the width of the sprite
  void setWidth(int width);
  // Get the width of the sprite
  [[nodiscard]] auto getWidth() const -> int;

  // Set the height of the sprite
  void setHeight(int height);
  // Get the height of the sprite
  [[nodiscard]] auto getHeight() const -> int;

  // Set the color of the sprite
  void setColor(Color color);
  // Get the color of the sprite
  [[nodiscard]] auto getColor() const -> Color;

  // Set the label of the sprite
  void setLabel(string label);
  // Get the label of the sprite
  [[nodiscard]] auto getLabel() const -> string;

  // Set the number of frames to wait before changing frame
  void setSlowdown(int slowdown);
  // Get the number of frames to wait before changing frame
  [[nodiscard]] auto getSlowdown() const -> int;

  // Set the character to use for transparency
  void setTransparencyCharachter(char character);
  // Get the character to use for transparency
  [[nodiscard]] auto getTransparencyCharchter() const -> char;

  // Add a frame to the sprite
  auto addFrame(Frame frame) -> int;
  // Get the frame at a given index from the sprite
  [[nodiscard]] auto getFrame(int index) const -> Frame;
  // Get the number of frames the sprite holds
  [[nodiscard]] auto getFrameCount() const -> int;
  // Draw the i-th frame of the sprite at the given position
  [[nodiscard]] auto draw(int i, Vector position) const -> int;
};

}  // namespace df

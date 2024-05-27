#pragma once

#include <string>
#include <vector>

#include "Frame.h"
#include "latebit/core/geometry/Vector.h"

using namespace std;

namespace lb {
enum Slowdown {
  STOANIMATION = 0,
  NO_SLOWDOWN = 1,
};

class Sprite {
 private:
  // The width of the sprite
  uint8_t width = 0;
  // The height of the sprite
  uint8_t height = 0;
  // The number of frames to wait before changing frame
  uint8_t slowdown = NO_SLOWDOWN;
  // The frames of the sprite
  vector<Frame> frames = {};
  // The label of the sprite, used to reference it
  string label = "";

 public:
  ~Sprite();
  // Creates a sprite with a given maximum number of frames
  Sprite(string label = "", uint8_t width = 0, uint8_t height = 0,
         uint8_t slowdown = NO_SLOWDOWN, uint8_t frameCount = 0);
  // Creates a sprite from another sprite
  Sprite(const Sprite& other);
  // Checks if two sprites are equal
  auto operator==(const Sprite& other) const -> bool;

  // Set the width of the sprite
  void setWidth(uint8_t width);
  // Get the width of the sprite
  [[nodiscard]] auto getWidth() const -> uint8_t;

  // Set the height of the sprite
  void setHeight(uint8_t height);
  // Get the height of the sprite
  [[nodiscard]] auto getHeight() const -> uint8_t;

  // Set the label of the sprite
  void setLabel(string label);
  // Get the label of the sprite
  [[nodiscard]] auto getLabel() const -> string;

  // Set the number of frames to wait before changing frame
  void setSlowdown(uint8_t slowdown);
  // Get the number of frames to wait before changing frame
  [[nodiscard]] auto getSlowdown() const -> uint8_t;

  // Add a frame to the sprite
  auto addFrame(Frame frame) -> int;
  // Get the frame at a given index from the sprite
  [[nodiscard]] auto getFrame(int index) const -> Frame;
  // Get the number of frames the sprite holds
  [[nodiscard]] auto getFrameCount() const -> uint8_t;
  // Draw the i-th frame of the sprite at the given position
  [[nodiscard]] auto draw(int i, Vector position) const -> int;
};

}  // namespace lb

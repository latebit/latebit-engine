#pragma once

#include <string>
#include <vector>

#include "Keyframe.h"
#include "latebit/core/geometry/Vector.h"

using namespace std;

namespace lb {
const uint8_t NO_ANIMATION = 0;

class Sprite {
 private:
  // The width of the sprite
  uint8_t width = 0;
  // The height of the sprite
  uint8_t height = 0;
  // Duration of frames in FPS.
  // For example, if the game runs at 60FPS, duration = 60
  // means that the sprite will change frame every second
  uint8_t duration = 1;
  // The label of the sprite, used to reference it
  string label = "";
  // The frames of the sprite
  vector<Keyframe> frames = {};

 public:
  ~Sprite();
  // Creates a sprite with a given maximum number of frames
  Sprite(string label = "", uint8_t width = 0, uint8_t height = 0,
         uint8_t duration = 1, uint8_t frameCount = 0);
  // Creates a sprite from another sprite
  Sprite(const Sprite& other);
  // Checks if two sprites are equal
  auto operator==(const Sprite& other) const -> bool;

  // Get the width of the sprite
  [[nodiscard]] auto getWidth() const -> uint8_t;

  // Get the height of the sprite
  [[nodiscard]] auto getHeight() const -> uint8_t;

  // Set the label of the sprite
  void setLabel(string label);
  // Get the label of the sprite
  [[nodiscard]] auto getLabel() const -> string;

  // Get the number of frames to wait before changing frame
  [[nodiscard]] auto getDuration() const -> uint8_t;

  // Add a frame to the sprite
  auto addFrame(Keyframe frame) -> int;
  // Get the frame at a given index from the sprite
  [[nodiscard]] auto getFrame(int index) const -> Keyframe;
  // Set the frame at a given index from the sprite
  auto setFrame(int index, Keyframe frame) -> int;

  // Get the number of frames the sprite holds
  [[nodiscard]] auto getFrameCount() const -> uint8_t;

  // Draw the i-th frame of the sprite at the given position
  [[nodiscard]] auto draw(int index, Vector position) const -> int;
};

}  // namespace lb

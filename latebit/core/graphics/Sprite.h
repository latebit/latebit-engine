#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "Keyframe.h"
#include "latebit/core/geometry/Vector.h"

using namespace std;

namespace lb {
const uint8_t NO_ANIMATION = 0;

class Sprite {
 private:
  // The width of the sprite
  const uint8_t width = 0;
  // The height of the sprite
  const uint8_t height = 0;
  // Duration of frames in FPS.
  // For example, if the game runs at 60FPS, duration = 60
  // means that the sprite will change frame every second
  const uint8_t duration = 1;
  // The label of the sprite, used to reference it
  const string label = "";
  // The frames of the sprite
  const vector<Keyframe> frames = {};

 public:
  // Creates a sprite with the given parameters
  Sprite(string label = "", uint8_t width = 0, uint8_t height = 0,
         uint8_t duration = 1, vector<Keyframe> frames = {})
    : width(width),
      height(height),
      duration(duration),
      label(std::move(label)),
      frames(std::move(frames)) {};

  virtual ~Sprite() = default;

  // Checks if two sprites are equal
  auto operator==(const Sprite &other) const -> bool;

  // Get the width of the sprite
  [[nodiscard]] auto getWidth() const -> uint8_t;

  // Get the height of the sprite
  [[nodiscard]] auto getHeight() const -> uint8_t;

  // Get the label of the sprite
  [[nodiscard]] auto getLabel() const -> string;

  // Get the number of frames to wait before changing frame
  [[nodiscard]] auto getDuration() const -> uint8_t;

  // Get the frame at a given index from the sprite
  [[nodiscard]] auto getFrame(int index) const -> Keyframe;

  // Get the number of frames the sprite holds
  [[nodiscard]] auto getFrameCount() const -> uint8_t;

  // Draw the i-th frame of the sprite at the given position
  [[nodiscard]] auto virtual drawKeyframe(int index, Vector position,
                                          uint8_t scale = 1) const -> int;
};

}  // namespace lb

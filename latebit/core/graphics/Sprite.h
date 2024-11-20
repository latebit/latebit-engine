#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Keyframe.h"
#include "latebit/core/geometry/Vector.h"

using namespace std;

namespace lb {
const int NO_ANIMATION = 0;

class Sprite {
 private:
  // The width of the sprite
  const int width = 0;
  // The height of the sprite
  const int height = 0;
  // Duration of frames in FPS.
  // For example, if the game runs at 60FPS, duration = 60
  // means that the sprite will change frame every second
  const int duration = 1;
  // The label of the sprite, used to reference it
  const string label = "";
  // The frames of the sprite
  const vector<Keyframe> frames = {};

 public:
  // Creates a sprite with the given parameters
  Sprite(string label = "", int width = 0, int height = 0,
         int duration = 1, vector<Keyframe> frames = {})
    : width(width),
      height(height),
      duration(duration),
      label(std::move(label)),
      frames(std::move(frames)) {};

  virtual ~Sprite() = default;

  // Checks if two sprites are equal
  auto operator==(const Sprite &other) const -> bool;

  // Get the width of the sprite
  [[nodiscard]] auto getWidth() const -> int;

  // Get the height of the sprite
  [[nodiscard]] auto getHeight() const -> int;

  // Get the label of the sprite
  [[nodiscard]] auto getLabel() const -> string;

  // Get the number of frames to wait before changing frame
  [[nodiscard]] auto getDuration() const -> int;

  // Get the frame at a given index from the sprite
  [[nodiscard]] auto getFrame(int index) const -> Keyframe;

  // Get the number of frames the sprite holds
  [[nodiscard]] auto getFrameCount() const -> int;

  // Draw the i-th frame of the sprite at the given position
  [[nodiscard]] auto virtual drawKeyframe(int index, Vector position,
                                          int scale = 1) const -> int;
};

}  // namespace lb

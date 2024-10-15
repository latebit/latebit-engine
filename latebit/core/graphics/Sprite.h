#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "Keyframe.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/DisplayManager.h"

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

  int (*drawKeyframeAtPosition)(Vector position,
                                const vector<Color::Color> *keyframe,
                                uint8_t width, uint8_t height, uint8_t scale);

 public:
  // Creates a sprite with the given parameters
  Sprite(string label = "", uint8_t width = 0, uint8_t height = 0,
         uint8_t duration = 1, vector<Keyframe> frames = {},
         int (*draw)(Vector, const vector<Color::Color> *, uint8_t, uint8_t,
                     uint8_t) = &DisplayManager::drawKeyframe)
    : width(width),
      height(height),
      duration(duration),
      label(std::move(label)),
      frames(std::move(frames)),
      drawKeyframeAtPosition(draw) {};

  ~Sprite() = default;

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
  [[nodiscard]] auto drawKeyframe(int index, Vector position,
                                  uint8_t scale = 1) const -> int;
};

}  // namespace lb

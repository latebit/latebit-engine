#pragma once

#include <string>
#include <vector>

#include "Colors.h"
#include "Frame.h"
#include "Vector.h"

namespace df {
enum Slowdown {
  STOANIMATION = 0,
  NO_SLOWDOWN = 1,
};

class Sprite {
 private:
  int width = 0;
  int height = 0;
  int maxFrameCount = 0;
  int frameCount = 0;
  int slowdown = NO_SLOWDOWN;
  Color color = COLOR_DEFAULT;
  std::vector<Frame> frames = {};
  std::string label = "";
  char transparencyChar = ' ';

 public:
  ~Sprite();
  Sprite(int maxFrames = 0);
  Sprite(const Sprite& other);
  auto operator==(const Sprite& other) const -> bool;

  void setWidth(int width);
  [[nodiscard]] auto getWidth() const -> int;

  void setHeight(int height);
  [[nodiscard]] auto getHeight() const -> int;

  void setColor(Color color);
  [[nodiscard]] auto getColor() const -> Color;

  void setLabel(std::string label);
  [[nodiscard]] auto getLabel() const -> std::string;

  void setSlowdown(int slowdown);
  [[nodiscard]] auto getSlowdown() const -> int;

  void setTransparencyChar(char transparency_char);
  [[nodiscard]] auto getTransparencyChar() const -> char;

  auto addFrame(Frame frame) -> int;
  [[nodiscard]] auto getFrame(int frame_number) const -> Frame;
  [[nodiscard]] auto getFrameCount() const -> int;

  [[nodiscard]] auto draw(int frame_number, Vector position) const -> int;
};

}  // namespace df

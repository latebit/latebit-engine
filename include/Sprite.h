#pragma once

#include <string>

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
  int width;
  int height;
  int max_frame_count;
  int frame_count;
  int slowdown;
  Color color;
  Frame* frames;
  std::string label;
  char transparency_char;
  Sprite();

 public:
  ~Sprite();
  Sprite(int max_frames);
  Sprite(const Sprite& other);

  void setWidth(int width);
  [[nodiscard]] auto getWidth() const -> int;

  void setHeight(int height);
  [[nodiscard]] auto getHeight() const -> int;

  void setColor(Color color);
  [[nodiscard]] auto getColor() const -> Color;

  void setLabel(std::string label);
  auto getLabel() const -> std::string;

  void setSlowdown(int slowdown);
  [[nodiscard]] auto getSlowdown() const -> int;

  void setTransparencyChar(char transparency_char);
  [[nodiscard]] auto getTransparencyChar() const -> char;

  [[nodiscard]] auto getFrameCount() const -> int;
  auto addFrame(Frame frame) -> int;
  [[nodiscard]] auto getFrame(int frame_number) const -> Frame;

  [[nodiscard]] auto draw(int frame_number, Vector position) const -> int;
};

}  // namespace df

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
  auto getWidth() const -> int;

  void setHeight(int height);
  auto getHeight() const -> int;

  void setColor(Color color);
  auto getColor() const -> Color;

  void setLabel(std::string label);
  std::string getLabel() const;

  void setSlowdown(int slowdown);
  auto getSlowdown() const -> int;

  void setTransparencyChar(char transparency_char);
  auto getTransparencyChar() const -> char;

  auto getFrameCount() const -> int;
  auto addFrame(Frame frame) -> int;
  auto getFrame(int frame_number) const -> Frame;

  auto draw(int frame_number, Vector position) const -> int;
};

}  // namespace df

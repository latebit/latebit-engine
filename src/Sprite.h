#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <string>

#include "Colors.h"
#include "Frame.h"
#include "Vector.h"

namespace df {
enum Slowdown {
  STOP_ANIMATION = 0,
  NO_SLOWDOWN = 1,
};

class Sprite {
 private:
  int m_width;
  int m_height;
  int m_max_frame_count;
  int m_frame_count;
  int m_slowdown;
  Color m_color;
  Frame* m_frame;
  std::string m_label;
  Sprite();

 public:
  ~Sprite();
  Sprite(int max_frames);

  void setWidth(int width);
  int getWidth() const;

  void setHeight(int height);
  int getHeight() const;

  void setColor(Color color);
  Color getColor() const;

  void setLabel(std::string label);
  std::string getLabel() const;

  void setSlowdown(int slowdown);
  int getSlowdown() const;

  int getFrameCount() const;
  int addFrame(Frame frame);
  Frame getFrame(int frame_number) const;

  int draw(int frame_number, Vector position) const;
};

}  // namespace df
#endif
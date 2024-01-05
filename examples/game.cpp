#include <thread>

#include "../includes/Colors.h"
#include "../includes/DisplayManager.h"
#include "../includes/EventKeyboard.h"
#include "../includes/EventOut.h"
#include "../includes/EventStep.h"
#include "../includes/GameManager.h"
#include "../includes/LogManager.h"
#include "../includes/Object.h"
#include "../includes/ResourceManager.h"
#include "../includes/Sprite.h"
#include "../includes/SpriteParser.h"
#include "../includes/Vector.h"
#include "../includes/WorldManager.h"
#include "../includes/utils.h"

int getRandom(int max, int min = 0) {
  int delta = max - min;
  return min + rand() % (delta + 1);
}

class Flake : public df::Object {
 private:
  void resetPositionAndVelocity() {
    setPosition(
        df::Vector(getRandom(DM.getHorizontalCells()), -1 * getRandom(5, 1)));
    setVelocity(df::Vector(0.1, 0.05 * getRandom(6)));
    m_step_check = getRandom(120, 1);
  }

  int m_step_check = 1;

 public:
  Flake() {
    resetPositionAndVelocity();
    setSolidness(df::SPECTRAL);
    setType("Flake");
  }

  int draw() {
    return DM.drawCh(getPosition(), '*', df::Color::WHITE,
                     df::Color::UNDEFINED_COLOR);
  }

  int eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::OUT_EVENT) {
      resetPositionAndVelocity();
      return 1;
    }
    if (p_e->getType() == df::STEP_EVENT) {
      auto p_event_step = dynamic_cast<const df::EventStep *>(p_e);

      auto steps = p_event_step->getStepCount();
      if (steps % m_step_check == 0) {
        df::Vector v = getVelocity();
        float x = v.getX() * -1;
        setVelocity(df::Vector(x, v.getY()));
      };

      return 1;
    }
    return 0;
  }
};

class Ship : public df::Object {
 private:
  int m_current_frame;
  df::Sprite *m_sprite;

 public:
  Ship() {
    setType("Ship");
    setSolidness(df::SPECTRAL);
    setAltitude(df::MAX_ALTITUDE - 1);
    setPosition(df::Vector(7, 7));
    m_current_frame = 0;
    m_sprite = RM.getSprite("sprite");
  }

  int eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::STEP_EVENT && m_sprite != nullptr &&
        m_sprite->getFrameCount() > 0) {
      auto p_event_step = dynamic_cast<const df::EventStep *>(p_e);
      if (p_event_step->getStepCount() % m_sprite->getSlowdown() == 0) {
        m_current_frame =
            p_event_step->getStepCount() % m_sprite->getFrameCount();
        return 1;
      }
    }

    return 0;
  }

  int draw() {
    if (m_sprite == nullptr) return -1;

    return m_sprite->draw(m_current_frame, getPosition());
  }
};

int main() {
  LM.setFlush(true);

  GM.startUp();

  if (RM.loadSprite("examples/sprite.txt", "sprite") != 0) {
    LM.writeLog("Failed to load sprite");
    return 1;
  }

  new Ship;

  GM.run();

  return 0;
}

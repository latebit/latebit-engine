#include "latebit/ui/Dialog.h"

#include <ctime>
#include <sstream>

#include "latebit/core/GameManager.h"
#include "latebit/core/events/EventInput.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/graphics/Font.h"
#include "latebit/core/world/Scene.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/ui/Rectangle.h"
#include "latebit/ui/Slider.h"
#include "latebit/ui/Text.h"
#include "latebit/ui/utils.h"
#include "latebit/utils/Logger.h"

using namespace lb;
using namespace lbui;

class GameScene : public Scene {
 private:
  Dialog* dialog;

 public:
  GameScene() : Scene() {
    DM.setBackground(Color::ORANGE);
    dialog = createObject<Dialog>(
      this, vector<string>{"It's dangerous to go alone!", "Take this!"}, []() {
        Log.info("Dialog ended");
      }, RectangleOptions{.height = 40, .width = WINDOW_WIDTH - 2,
                          .fill = Color::YELLOW,
                          .stroke = {Color::BLACK, Color::YELLOW, Color::BLUE }});
    dialog->setPosition({1, WINDOW_HEIGHT - 41});

    stringstream ss;
    ss << "Press " << (char)InputGlyph::A << " to see dialog";
    auto text = createObject<Text>(
      "Instructions", ss.str(),
      TextOptions{.size = TextSize::NORMAL, .color = Color::WHITE});
    setPositionOnScreen(text, ScreenPosition::MIDDLE_CENTER, Vector{0, -10});
    
    subscribe(INPUT_EVENT);
  }

  auto eventHandler(const Event* event) -> int override {
    if (event->getType() == INPUT_EVENT) {
      const auto inputEvent = static_cast<const EventInput*>(event);
      if (inputEvent->getAction() == InputAction::PRESSED &&
          inputEvent->getKey() == InputKey::A) {
        dialog->setActive(true);
      }
    }
    return 0;
  }
};

auto main() -> int {
  Log.setLevel(LogLevel::DEBUG);
  GM.startUp();

  auto scene = WM.createScene<GameScene>("Scene");
  scene->activate();

  GM.run();

  return 0;
}
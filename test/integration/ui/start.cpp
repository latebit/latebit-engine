#include <cstdio>
#include <ctime>

#include "latebit/core/GameManager.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/Scene.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/ui/Button.h"
#include "latebit/ui/Checkbox.h"
#include "latebit/ui/Menu.h"
#include "latebit/ui/Rectangle.h"
#include "latebit/ui/Text.h"
#include "latebit/ui/utils.h"
#include "latebit/utils/Logger.h"

using namespace lb;
using namespace lbui;

auto main() -> int {
  Log.setLevel(LogLevel::DEBUG);
  GM.startUp();

  auto scene = WM.createScene<Scene>("Scene");
  DM.setBackground(Color::BLUE);
  auto title = scene->createObject<Text>(
    "title", "BEST GAME",
    TextOptions{.size = TextSize::XXLARGE, .shadow = lb::Color::DARK_BLUE});
  setPositionOnScreen(title, ScreenPosition::TOP_CENTER, Vector{0, 10});

  auto menu = scene->createObject<Menu>();

  auto start = scene->createObject<Button>(scene, "Start Game",
                                           []() { printf("Start\n"); });
  Checkbox* checkbox = nullptr;
  checkbox = scene->createObject<Checkbox>(
    scene, "Options", false,
    [&checkbox]() { printf("Options %d\n", checkbox->getValue()); });
  menu->addControl(start);
  menu->addControl(checkbox);
  setPositionOnScreen(menu, ScreenPosition::MIDDLE_CENTER);

  auto rect = scene->createObject<Rectangle>(
    RectangleOptions{.height = 40,
                     .width = 120,
                     .fill = Color::WHITE,
                     .stroke = {Color::BLACK, Color::UNDEFINED_COLOR, Color::BLACK, Color::DARK_GRAY }});
  setPositionOnScreen(rect, ScreenPosition::BOTTOM_CENTER, Vector{0, -10});

  scene->activate();

  GM.run();

  return 0;
}
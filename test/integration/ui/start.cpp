#include <cstdio>
#include <ctime>

#include "latebit/core/GameManager.h"
#include "latebit/core/geometry/Vector.h"
#include "latebit/core/graphics/Colors.h"
#include "latebit/core/graphics/DisplayManager.h"
#include "latebit/core/world/Scene.h"
#include "latebit/core/world/WorldManager.h"
#include "latebit/ui/Button.h"
#include "latebit/ui/Menu.h"
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

  auto start = scene->createObject<Button>(scene, "Start Game", []() { printf("Start\n"); });
  auto options = scene->createObject<Button>(scene, "Options", []() { printf("Options\n"); });
  menu->addControl(start);
  menu->addControl(options);
  setPositionOnScreen(menu, ScreenPosition::MIDDLE_CENTER);

  scene->activate();

  GM.run();

  return 0;
}
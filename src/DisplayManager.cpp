#include "DisplayManager.h"

#include "LogManager.h"
#include "utils.h"

namespace df {

DisplayManager::DisplayManager() {
  setType("DisplayManager");
  LM.writeLog("DisplayManager::DisplayManager(): Created DisplayManager");
}

auto DisplayManager::getInstance() -> DisplayManager& {
  static DisplayManager instance;
  return instance;
}

auto DisplayManager::startUp() -> int {
  if (this->window != nullptr) {
    return 0;
  }

  auto mode = sf::VideoMode(this->widthInPixels, this->heightInPixels);
  this->window =
    new sf::RenderWindow(mode, WINDOW_TITLE_DEFAULT, WINDOW_STYLE_DEFAULT);

  this->window->setMouseCursorVisible(false);
  this->window->setVerticalSyncEnabled(true);

  sf::Font font;
  if (font.loadFromFile(FONT_FILE_DEFAULT) == false) {
    LM.writeLog("DisplayManager::startUp(): Warning! Font file not found");
    return -1;
  }
  this->font = font;

  LM.writeLog("DisplayManager::startUp(): Started successfully");
  return Manager::startUp();
}

void DisplayManager::shutDown() {
  this->window->close();
  delete this->window;
  Manager::shutDown();
  LM.writeLog("DisplayManager::shutDown(): Shut down successfully");
}

auto DisplayManager::drawCh(Vector worldPosition, char ch, Color fg,
                            Color bg) const -> int {
  if (this->window == nullptr) return -1;

  auto viewPosition = worldToView(worldPosition);
  auto pixelPosition = cellsToPixels(viewPosition);

  static auto width = charWidth();
  static auto height = charHeight();

  if (bg != UNDEFINED_COLOR) {
    static sf::RectangleShape background;
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(toSFColor(bg));
    background.setPosition(pixelPosition.getX(), pixelPosition.getY());
    this->window->draw(background);
  }

  static sf::Text text("", this->font);
  text.setString(ch);
  text.setStyle(sf::Text::Bold);

  if (width < height) {
    text.setCharacterSize(width * 2);
  } else {
    text.setCharacterSize(height * 2);
  }

  text.setFillColor(toSFColor(fg));
  text.setPosition(pixelPosition.getX(), pixelPosition.getY());

  this->window->draw(text);

  return 0;
}

auto DisplayManager::drawCh(Vector worldPosition, char ch, Color fg) const
  -> int {
  return DisplayManager::drawCh(worldPosition, ch, fg, this->backgroundColor);
}

auto DisplayManager::drawString(Vector worldPosition, std::string s,
                                Alignment a, Color fg, Color bg) const -> int {
  Vector start = worldPosition;
  switch (a) {
    case ALIGN_CENTER:
      start.setX(worldPosition.getX() - s.size() / 2);
      break;
    case ALIGN_RIGHT:
      start.setX(worldPosition.getX() - s.size());
      break;
    case ALIGN_LEFT:
    default:
      break;
  }

  for (int i = 0; i < s.size(); i++) {
    Vector pos(start.getX() + i, start.getY());
    if (drawCh(pos, s[i], fg, bg) != 0) {
      return -1;
    }
  }

  return 0;
}

auto DisplayManager::drawString(Vector worldPosition, std::string s,
                                Alignment a, Color fg) const -> int {
  return DisplayManager::drawString(worldPosition, s, a, fg,
                                    this->backgroundColor);
}

void DisplayManager::setBackground(Color color) {
  this->backgroundColor = color;
}

auto DisplayManager::getHorizontalCells() const -> int {
  return this->widthInCells;
}

auto DisplayManager::getVerticalCells() const -> int {
  return this->heightInCells;
}

auto DisplayManager::getHorizontalPixels() const -> int {
  return this->widthInPixels;
}

auto DisplayManager::getVerticalPixels() const -> int {
  return this->heightInPixels;
}

auto DisplayManager::swapBuffers() -> int {
  if (this->window == nullptr) {
    LM.writeLog("DisplayManager::swapBuffers(): Window is null");
    return -1;
  }

  // displays current buffer
  this->window->display();

  // clears second buffer
  this->window->clear(toSFColor(this->backgroundColor));

  return 0;
}

auto DisplayManager::getWindow() const -> sf::RenderWindow* {
  return this->window;
}

auto charHeight() -> float {
  return DM.getVerticalPixels() / (float)DM.getVerticalCells();
}

auto charWidth() -> float {
  return DM.getHorizontalPixels() / (float)DM.getHorizontalCells();
}

auto cellsToPixels(Vector spaces) -> Vector {
  return {spaces.getX() * charWidth(), spaces.getY() * charHeight()};
}

auto pixelsToCells(Vector pixels) -> Vector {
  return {pixels.getX() / charWidth(), pixels.getY() / charHeight()};
}

}  // namespace df

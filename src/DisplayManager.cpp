#include "DisplayManager.h"

#include "LogManager.h"

namespace df {

DisplayManager::DisplayManager() {
  setType("DisplayManager");
  this->window_horizontal_pixels = WINDOW_HORIZONTAL_PIXELS_DEFAULT;
  this->window_horizontal_cells = WINDOW_HORIZONTAL_CHARS_DEFAULT;
  this->window_vertical_cells = WINDOW_VERTICAL_CHARS_DEFAULT;
  this->window_vertical_pixels = WINDOW_VERTICAL_PIXELS_DEFAULT;
  this->background_color = WINDOW_BACKGROUND_COLOR_DEFAULT;
  this->window = NULL;
  LM.writeLog("DisplayManager::DisplayManager(): Created DisplayManager");
}

DisplayManager& DisplayManager::getInstance() {
  static DisplayManager instance;
  return instance;
}

int DisplayManager::startUp() {
  if (this->window != nullptr) {
    return 0;
  }

  auto mode =
      sf::VideoMode(this->window_horizontal_pixels, this->window_vertical_pixels);
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

int DisplayManager::drawCh(Vector worldPosition, char ch, Color fg,
                           Color bg) const {
  if (this->window == nullptr) return -1;

  auto pixelPosition = spacesToPixels(worldPosition);

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

int DisplayManager::drawCh(Vector world_pos, char ch, Color fg) const {
  return DisplayManager::drawCh(world_pos, ch, fg, this->background_color);
}

int DisplayManager::drawString(Vector world_pos, std::string s, Alignment a,
                               Color fg, Color bg) const {
  Vector start = world_pos;
  switch (a) {
    case ALIGN_CENTER:
      start.setX(world_pos.getX() - s.size() / 2);
      break;
    case ALIGN_RIGHT:
      start.setX(world_pos.getX() - s.size());
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

int DisplayManager::drawString(Vector world_pos, std::string s, Alignment a,
                               Color fg) const {
  return DisplayManager::drawString(world_pos, s, a, fg, this->background_color);
}

void DisplayManager::setBackground(Color color) { this->background_color = color; }

int DisplayManager::getHorizontalCells() const {
  return this->window_horizontal_cells;
}

int DisplayManager::getVerticalCells() const { return this->window_vertical_cells; }

int DisplayManager::getHorizontalPixels() const {
  return this->window_horizontal_pixels;
}

int DisplayManager::getVerticalPixels() const {
  return this->window_vertical_pixels;
}

int DisplayManager::swapBuffers() {
  if (this->window == nullptr) {
    LM.writeLog("DisplayManager::swapBuffers(): Window is null");
    return -1;
  }

  // displays current buffer
  this->window->display();

  // clears second buffer
  this->window->clear(toSFColor(this->background_color));

  return 0;
}

sf::RenderWindow* DisplayManager::getWindow() const { return this->window; }

float charHeight() {
  return DM.getVerticalPixels() / (float)DM.getVerticalCells();
}

float charWidth() {
  return DM.getHorizontalPixels() / (float)DM.getHorizontalCells();
}

Vector spacesToPixels(Vector spaces) {
  return Vector(spaces.getX() * charWidth(), spaces.getY() * charHeight());
}

Vector pixelsToSpaces(Vector pixels) {
  return Vector(pixels.getX() / charWidth(), pixels.getY() / charHeight());
}

}  // namespace df

#include "DisplayManager.h"

namespace df {

DisplayManager::DisplayManager() {
  setType("DisplayManager");
  m_window_horizontal_pixels = WINDOW_HORIZONTAL_PIXELS_DEFAULT;
  m_window_horizontal_chars = WINDOW_HORIZONTAL_CHARS_DEFAULT;
  m_window_vertical_chars = WINDOW_VERTICAL_CHARS_DEFAULT;
  m_window_vertical_pixels = WINDOW_VERTICAL_PIXELS_DEFAULT;
  m_background_color = WINDOW_BACKGROUND_COLOR_DEFAULT;
  m_p_window = NULL;
}

DisplayManager& DisplayManager::getInstance() {
  static DisplayManager instance;
  return instance;
}

int DisplayManager::startUp() {
  if (m_p_window != nullptr) {
    return 0;
  }

  auto mode =
      sf::VideoMode(m_window_horizontal_pixels, m_window_vertical_pixels);
  m_p_window =
      new sf::RenderWindow(mode, WINDOW_TITLE_DEFAULT, WINDOW_STYLE_DEFAULT);

  m_p_window->setMouseCursorVisible(false);
  m_p_window->setVerticalSyncEnabled(true);

  sf::Font font;
  if (font.loadFromFile(FONT_FILE_DEFAULT) == false) {
    return -1;
  }
  m_font = font;
  return Manager::startUp();
}

void DisplayManager::shutDown() {
  m_p_window->close();
  delete m_p_window;
  Manager::shutDown();
}

int DisplayManager::drawCh(Vector world_pos, char ch, Color color) const {
  if (m_p_window == nullptr) {
    return -1;
  }

  Vector pixel_pos = spacesToPixels(world_pos);

  auto width = charWidth();
  auto height = charHeight();

  static sf::RectangleShape background;
  background.setSize(sf::Vector2f(width, height));
  background.setFillColor(toSFColor(m_background_color));
  background.setPosition(pixel_pos.getX() - width / 10,
                         pixel_pos.getY() + height / 5);
  m_p_window->draw(background);

  static sf::Text text("", m_font);
  text.setString(ch);
  text.setStyle(sf::Text::Bold);

  if (width < height) {
    text.setCharacterSize(width * 2);
  } else {
    text.setCharacterSize(height * 2);
  }

  text.setFillColor(toSFColor(color));
  text.setPosition(pixel_pos.getX(), pixel_pos.getY());

  m_p_window->draw(text);

  return 0;
}

int DisplayManager::drawString(Vector world_pos, std::string s, Alignment a,
                               Color color) const {
  Vector start = world_pos;
  switch (a) {
    case ALIGN_CENTER:
      start.setX(world_pos.getX() - s.size() / 2);
    case ALIGN_RIGHT:
      start.setX(world_pos.getX() - s.size());
    case ALIGN_LEFT:
    default:
      break;
  }

  for (int i = 0; i < s.size(); i++) {
    Vector pos(start.getX() + i, start.getY());
    if (drawCh(pos, s[i], color) != 0) {
      return -1;
    }
  }

  return 0;
}

void DisplayManager::setBackground(Color color) { m_background_color = color; }

int DisplayManager::getHorizontal() const { return m_window_horizontal_chars; }

int DisplayManager::getVertical() const { return m_window_vertical_chars; }

int DisplayManager::getHorizontalPixels() const {
  return m_window_horizontal_pixels;
}

int DisplayManager::getVerticalPixels() const {
  return m_window_vertical_pixels;
}

int DisplayManager::swapBuffers() {
  if (m_p_window == nullptr) return -1;

  // displays current buffer
  m_p_window->display();

  // clears second buffer
  m_p_window->clear(toSFColor(m_background_color));

  return 0;
}

sf::RenderWindow* DisplayManager::getWindow() const { return m_p_window; }

float charHeight() { return DM.getVerticalPixels() / DM.getVertical(); }

float charWidth() { return DM.getHorizontalPixels() / DM.getHorizontal(); }

Vector spacesToPixels(Vector spaces) {
  return Vector(spaces.getX() * charWidth(), spaces.getY() * charHeight());
}

Vector pixelsToSpaces(Vector pixels) {
  return Vector(pixels.getX() / charWidth(), pixels.getY() / charHeight());
}

}  // namespace df

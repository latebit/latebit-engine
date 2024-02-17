#include "DisplayManager.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Colors.h"
#include "Frame.h"
#include "LogManager.h"
#include "Vector.h"
#include "utils.h"

namespace lb {

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

  auto widthInPixels = this->widthInCells * CELL_SIZE;
  auto heightInPixels = this->heightInCells * CELL_SIZE;
  auto mode = sf::VideoMode(widthInPixels, heightInPixels);
  this->window = new sf::RenderWindow(mode, WINDOW_TITLE_DEFAULT, WINDOW_STYLE);

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
  this->window = nullptr;
  Manager::shutDown();
  LM.writeLog("DisplayManager::shutDown(): Shut down successfully");
}

auto DisplayManager::drawFrame(Position position, const Frame* frame) const
  -> int {
  if (this->window == nullptr) return -1;

  auto viewPosition = worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);
  auto content = frame->getContent();

  sf::VertexArray cells(sf::Quads);

  for (int i = 0; i < frame->getHeight(); i++) {
    for (int j = 0; j < frame->getWidth(); j++) {
      auto index = i * frame->getWidth() + j;

      float posX = pixelPosition.getX() + j * CELL_SIZE;
      float posY = pixelPosition.getY() + i * CELL_SIZE;

      // Define the four corners of the quad
      sf::Vertex topLeft(sf::Vector2f(posX, posY));
      sf::Vertex topRight(sf::Vector2f(posX + CELL_SIZE, posY));
      sf::Vertex bottomRight(sf::Vector2f(posX + CELL_SIZE, posY + CELL_SIZE));
      sf::Vertex bottomLeft(sf::Vector2f(posX, posY + CELL_SIZE));

      // Set the color for the cell (quad)
      sf::Color cellColor = toSFColor(content[index]);
      topLeft.color = cellColor;
      topRight.color = cellColor;
      bottomRight.color = cellColor;
      bottomLeft.color = cellColor;

      // Add the vertices to the VertexArray
      cells.append(topLeft);
      cells.append(topRight);
      cells.append(bottomRight);
      cells.append(bottomLeft);
    }
  }

  window->draw(cells);

  return 0;
}

auto DisplayManager::drawRectangle(Position position, int width, int height,
                                   Color borderColor, Color fillColor) const
  -> int {
  if (this->window == nullptr) return -1;

  auto viewPosition = worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);

  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(width * CELL_SIZE, height * CELL_SIZE));
  rectangle.setOutlineColor(toSFColor(borderColor));
  rectangle.setOutlineThickness(1);
  rectangle.setFillColor(toSFColor(fillColor));
  rectangle.setPosition(pixelPosition.getX(), pixelPosition.getY());

  this->window->draw(rectangle);

  return 0;
}

auto DisplayManager::drawRectangle(Position position, int width, int height,
                                   Color borderColor) const -> int {
  return drawRectangle(position, width, height, borderColor, UNDEFINED_COLOR);
}

auto DisplayManager::drawString(Position position, string string,
                                Alignment alignment, Color color) const -> int {
  if (this->window == nullptr) return -1;

  this->window->draw(makeText(position, string, alignment, color));

  return 0;
}

auto DisplayManager::measureString(string string) const -> Box {
  auto text = makeText({0, 0}, string, ALIGN_LEFT, UNDEFINED_COLOR);
  auto bounds = text.getGlobalBounds();
  auto cellBounds = pixelsToCells({bounds.width, bounds.height});
  auto position = pixelsToCells({bounds.left, bounds.top});

  // Adding the position to height and width allows this object to return the
  // real rectangle around the text (that takes into account the whitespace
  // added around the characters by the font itself). For example, when you have
  // "g" this trick makes room for the tail of the "g" character
  return {cellBounds.getX() + position.getX(),
          cellBounds.getY() + position.getY()};
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

auto DisplayManager::makeText(Position position, string string,
                              Alignment alignment, Color color) const
  -> sf::Text {
  sf::Text text(string, this->font);
  auto viewPosition = worldToView(position);
  auto pixelPosition = cellsToPixels(viewPosition);
  text.setCharacterSize(FONT_SIZE_DEFAULT);
  text.setFillColor(toSFColor(color));
  auto width = text.getLocalBounds().width;

  switch (alignment) {
    case ALIGN_CENTER:
      text.setPosition(pixelPosition.getX() - width / 2, pixelPosition.getY());
      break;
    case ALIGN_RIGHT:
      text.setPosition(pixelPosition.getX() - width, pixelPosition.getY());
      break;
    case ALIGN_LEFT:
      text.setPosition(pixelPosition.getX(), pixelPosition.getY());
      break;
  }

  return text;
}

auto cellsToPixels(Position spaces) -> Vector {
  return {spaces.getX() * CELL_SIZE, spaces.getY() * CELL_SIZE};
}

auto pixelsToCells(Vector pixels) -> Position {
  return {pixels.getX() / CELL_SIZE, pixels.getY() / CELL_SIZE};
}

}  // namespace lb

#include "Text.h"

void prepText(sf::Text& text, std::string txt, sf::Font& font, Rectangle rectangle) {
  text.setString(txt);
  text.setFont(font);
  text.setFillColor(sf::Color::White);
  sf::FloatRect textBounds = text.getLocalBounds();
  float scaleX = (rectangle.getColumnMax() - rectangle.getColumnMin()) / textBounds.width;
  float scaleY = -(rectangle.getRowMax() - rectangle.getRowMin()) / textBounds.height;
  text.setScale(sf::Vector2f(scaleX, scaleY));
  float x = rectangle.getColumnMin(), y = rectangle.getRowMax();
  sf::FloatRect r = text.getGlobalBounds();
  text.setPosition(x - r.left, y - r.top - (rectangle.getRowMax() - rectangle.getRowMin()));
}
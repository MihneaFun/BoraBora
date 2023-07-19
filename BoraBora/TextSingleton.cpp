#include "TextSingleton.h"

TextSingleton* TextSingleton::m_instance = nullptr;

TextSingleton* TextSingleton::getInstance() {
  if (!m_instance) {
    m_instance = new TextSingleton();
  }
  return m_instance;
}

void TextSingleton::createInstance() {
  if (!m_instance) {
    m_instance = new TextSingleton();
  }
}

void TextSingleton::destroyInstance() {
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

void TextSingleton::setFont(const sf::Font& font) {
  m_font = font;
}

void TextSingleton::setRectangle(const Rectangle& rectangle) {
  m_rectangle = rectangle;
}

sf::Text TextSingleton::getText(std::string txt) const {
  sf::Text text;
  text.setString(txt);
  text.setFont(m_font);
  text.setFillColor(sf::Color::White);
  sf::FloatRect textBounds = text.getLocalBounds();
  float scaleX = (m_rectangle.getColumnMax() - m_rectangle.getColumnMin()) / textBounds.width;
  float scaleY = -(m_rectangle.getRowMax() - m_rectangle.getRowMin()) / textBounds.height;
  text.setScale(sf::Vector2f(scaleX, scaleY));
  float x = m_rectangle.getColumnMin(), y = m_rectangle.getRowMax();
  sf::FloatRect r = text.getGlobalBounds();
  text.setPosition(x - r.left, y - r.top - (m_rectangle.getRowMax() - m_rectangle.getRowMin()));
  return text;
}

std::string TextSingleton::numToString(int num) {
  if (num == -1) {
    return "";
  }
  assert(num >= 0);
  if (num == 0) {
    return "0";
  }
  assert(num >= 1);
  std::string s;
  while (num) {
    s.push_back((char)('0' + num % 10));
    num /= 10;
  }
  std::reverse(s.begin(), s.end());
  return s;
}

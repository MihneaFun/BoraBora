#pragma once
#include <SFML/Graphics.hpp>
#include "Rectangle.h"
#include <string>
#include <cassert>
#include <algorithm>
#include <iostream>

class TextSingleton {
private:
  static TextSingleton* m_instance;
  sf::Font m_font;
  Rectangle m_rectangle;

  TextSingleton() :
    m_rectangle(0, 0, 0, 0) {

    if (!m_font.loadFromFile("font.ttf")) {
      std::cout << "failed loading font\n";
      exit(0);
    }
  }
  TextSingleton(const TextSingleton& other) = delete;
  TextSingleton(TextSingleton&& other) = delete;
  TextSingleton& operator=(const TextSingleton& other) = delete;
  TextSingleton& operator=(TextSingleton&& other) = delete;

public:
  static TextSingleton* getInstance();
  static void createInstance();
  static void destroyInstance();

  void setFont(const sf::Font& font);
  void setRectangle(const Rectangle& rectangle);
  sf::Text getText(std::string txt) const;
  std::string numToString(int num);
};


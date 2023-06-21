#pragma once

#include <SFML/Graphics.hpp>
#include "Rectangle.h"
#include "WorldBlocksSingleton.h"
#include "TextureAtlasSingleton.h"

class WorldDrawerSingleton {
private:
  static WorldDrawerSingleton* r_WorldDrawerSingleton;
  sf::RenderWindow& m_window;
  sf::VertexArray m_vertexArray;
  sf::RenderTexture m_renderTexture;  // New member here


  WorldDrawerSingleton(sf::RenderWindow& window);
  WorldDrawerSingleton(const WorldDrawerSingleton& other) = delete;
  WorldDrawerSingleton(WorldDrawerSingleton&& other) = delete;

public:
  static WorldDrawerSingleton* getInstance(sf::RenderWindow& window);
  static void createInstance(sf::RenderWindow& window);
  static void destroy();
  void drawWorldOnWindow(Rectangle windowRectangle, Rectangle worldRectangle);
};

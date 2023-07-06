#pragma once

#include <SFML/Graphics.hpp>
#include "Rectangle.h"

class Camera {
private:
  sf::Vector2f m_center;
  sf::Vector2f m_size;

  void updateView();

  sf::RenderTexture m_texture;

public:

  void create(int dimx, int dimy);
  void clear();

  const sf::Vector2f getCenter();
  const sf::Vector2f getSize();

  void setCenter(sf::Vector2f center);
  void setSize(sf::Vector2f size);

  void draw(const sf::Drawable& drawable, sf::RenderStates renderState = sf::RenderStates::Default);
  void drawCamera(Rectangle rectangle, sf::RenderTarget& renderTarget, sf::RenderStates renderState = sf::RenderStates::Default);
};

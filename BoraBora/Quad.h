#pragma once

#include <SFML/Graphics.hpp>

class Quad {
public:
  Quad();
  Quad(sf::Vertex v0, sf::Vertex v1, sf::Vertex v2, sf::Vertex v3);
  ~Quad();

  sf::Vertex v0;
  sf::Vertex v1;
  sf::Vertex v2;
  sf::Vertex v3;
};

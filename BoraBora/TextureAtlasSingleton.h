#pragma once

#include <SFML/Graphics.hpp>
#include "Rectangle.h"
#include "GeneralTextureManagerSingleton.h"

class TextureAtlasSingleton {
private:
  static TextureAtlasSingleton* m_instance;
  sf::Texture m_textureBand;
  std::vector<Rectangle> m_textureRectangles;

  TextureAtlasSingleton() = default;
  TextureAtlasSingleton(const TextureAtlasSingleton& other) = delete;
  TextureAtlasSingleton(TextureAtlasSingleton&& other) = delete;

public:
  static TextureAtlasSingleton* getInstance();
  static void destroy();
  static void createInstance();

  const sf::Texture& getTextureBand() const;
  Rectangle getTextureRectangle(TextureType type) const;
  void createTextureBand();
};

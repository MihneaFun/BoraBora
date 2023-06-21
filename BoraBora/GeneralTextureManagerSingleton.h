#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GeneralTextureLinkSingleton.h"
#include "TextureType.h"

class GeneralTextureManagerSingleton {
private:
  static GeneralTextureManagerSingleton* m_instance;
  std::vector<sf::Texture> m_textureMap;

  GeneralTextureManagerSingleton();
  GeneralTextureManagerSingleton(const GeneralTextureManagerSingleton& other) = delete;
  GeneralTextureManagerSingleton(GeneralTextureManagerSingleton&& other) = delete;

public:
  static GeneralTextureManagerSingleton* getInstance();
  static void destroy();
  static void createInstance();

  sf::Texture& getTexture(TextureType type);
};

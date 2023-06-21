#include "GeneralTextureManagerSingleton.h"

GeneralTextureManagerSingleton* GeneralTextureManagerSingleton::m_instance = nullptr;

GeneralTextureManagerSingleton::GeneralTextureManagerSingleton() {
  m_textureMap.resize(static_cast<int>(TextureType::COUNT));
}

GeneralTextureManagerSingleton* GeneralTextureManagerSingleton::getInstance() {
  if (!m_instance) {
    createInstance();
  }
  return m_instance;
}

void GeneralTextureManagerSingleton::destroy() {
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

void GeneralTextureManagerSingleton::createInstance() {
  if (!m_instance) {
    m_instance = new GeneralTextureManagerSingleton();
  }
}

sf::Texture& GeneralTextureManagerSingleton::getTexture(TextureType type) {
  int index = static_cast<int>(type);
  if (m_textureMap[index].getSize().x == 0) {  // Check if texture is loaded
    std::string textureLink = GeneralTextureLinkSingleton::getInstance()->getLink(type);
    if (!m_textureMap[index].loadFromFile(textureLink)) {
      throw std::runtime_error("Failed to load texture from: " + textureLink);
    }
  }
  return m_textureMap[index];
}

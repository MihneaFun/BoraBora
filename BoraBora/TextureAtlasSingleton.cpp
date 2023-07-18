#pragma once

#include "TextureAtlasSingleton.h"

TextureAtlasSingleton* TextureAtlasSingleton::m_instance = nullptr;

TextureAtlasSingleton* TextureAtlasSingleton::getInstance() {
  if (!m_instance) {
    createInstance();
  }
  return m_instance;
}

void TextureAtlasSingleton::destroy() {
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

void TextureAtlasSingleton::createInstance() {
  if (!m_instance) {
    m_instance = new TextureAtlasSingleton();
    m_instance->createTextureBand();
  }
}

const sf::Texture& TextureAtlasSingleton::getTextureBand() const {
  return m_textureBand;
}

Rectangle TextureAtlasSingleton::getTextureRectangle(TextureType type) const {
  if (static_cast<int>(type) < m_textureRectangles.size()) {
    return m_textureRectangles[static_cast<int>(type)];
  }
  else {
    // Error handling if texture rectangle couldn't be found
    throw std::runtime_error("Failed to get texture rectangle for texture type.");
  }
}


Rectangle TextureAtlasSingleton::getTextureRectangle(BlockType blockType) const {
  // shady
  // optimize 
  // optimise
  // here 
  // with a vector?
  // a vector is better than many ifs?

  TextureType type = TextureType::COUNT;

  if (blockType == BlockType::DIAMOND) type = TextureType::BLOCK_DIAMOND;
  if (blockType == BlockType::DIRT) type = TextureType::BLOCK_DIRT;
  if (blockType == BlockType::SAND) type = TextureType::BLOCK_SAND;
  if (blockType == BlockType::COAL) type = TextureType::BLOCK_COAL;
  if (blockType == BlockType::GRANITE) type = TextureType::BLOCK_GRANITE;
  if (blockType == BlockType::BLUE_WOOL) type = TextureType::BLOCK_BLUE_WOOL;
  if (blockType == BlockType::PINK_WOOL) type = TextureType::BLOCK_PINK_WOOL;
  if (blockType == BlockType::RED_WOOL) type = TextureType::BLOCK_RED_WOOL;
  if (blockType == BlockType::GREEN_WOOL) type = TextureType::BLOCK_GREEN_WOOL;
  if (blockType == BlockType::VOID) type = TextureType::BLOCK_VOID;

  assert(type != TextureType::COUNT);
  return getTextureRectangle(type);
}



void TextureAtlasSingleton::createTextureBand() {
  // Calculate total width and maximum height of the band image
  int totalWidth = 0;
  int maxHeight = 0;

  m_textureRectangles.clear();
  
  for (int i = 0; i < static_cast<int>(TextureType::COUNT); ++i) {
    TextureType type = static_cast<TextureType>(i);
    const sf::Texture& texture = GeneralTextureManagerSingleton::getInstance()->getTexture(type);
    totalWidth += texture.getSize().x;
    maxHeight = std::max(maxHeight, static_cast<int>(texture.getSize().y));
  }

  sf::Image imageBand;
  imageBand.create(totalWidth, maxHeight, sf::Color::Transparent);

  int offset = 0;

  // Loop through all TextureType values
  for (int i = 0; i < static_cast<int>(TextureType::COUNT); ++i) {
    TextureType type = static_cast<TextureType>(i);
    const sf::Texture& texture = GeneralTextureManagerSingleton::getInstance()->getTexture(type);

    sf::Image image = texture.copyToImage();
    imageBand.copy(image, offset, 0);

    int textureWidth = texture.getSize().x;
    int textureHeight = texture.getSize().y;

    m_textureRectangles.push_back(Rectangle(offset, offset + textureWidth, 0, textureHeight));

    offset += textureWidth;
  }

  m_textureBand.loadFromImage(imageBand);
}

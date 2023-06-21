#pragma once

#include "GeneralTextureLinkSingleton.h"

GeneralTextureLinkSingleton* GeneralTextureLinkSingleton::m_instance = nullptr;

GeneralTextureLinkSingleton* GeneralTextureLinkSingleton::getInstance() {
  if (!m_instance) {
    createInstance();
  }
  return m_instance;
}

void GeneralTextureLinkSingleton::destroy() {
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

void GeneralTextureLinkSingleton::createInstance() {
  if (!m_instance) {
    m_instance = new GeneralTextureLinkSingleton();
    m_instance->m_textureLinks.resize(static_cast<int>(TextureType::COUNT));
    GeneralTextureLinkSingleton::loadAllLinks();
  }
}

std::string GeneralTextureLinkSingleton::getLink(TextureType type) {
  int index = static_cast<int>(type);
  assert(index < m_textureLinks.size());
  return m_textureLinks[index];
}

void GeneralTextureLinkSingleton::setLink(TextureType type, const std::string& link) {
  int index = static_cast<int>(type);
  assert(index < m_textureLinks.size());
  m_textureLinks[index] = link;
}

void GeneralTextureLinkSingleton::loadAllLinks() {
  auto textureLinkSingleton = GeneralTextureLinkSingleton::getInstance();

  textureLinkSingleton->setLink(TextureType::BLOCK_DIAMOND, "textures/diamond_texture.png");
  textureLinkSingleton->setLink(TextureType::BLOCK_DIRT, "textures/unnamed.png");
  textureLinkSingleton->setLink(TextureType::BLOCK_VOID, "textures/void_texture.png");
}

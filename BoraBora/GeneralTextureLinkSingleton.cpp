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
  textureLinkSingleton->setLink(TextureType::BLOCK_DIRT, "textures/dirt_texture.png");
  textureLinkSingleton->setLink(TextureType::BLOCK_SAND, "textures/sand_texture.png");
  textureLinkSingleton->setLink(TextureType::BLOCK_VOID, "textures/void_texture.png");
  textureLinkSingleton->setLink(TextureType::BLOCK_COAL, "textures/coal_texture.png");
  textureLinkSingleton->setLink(TextureType::BLOCK_GRANITE, "textures/granite_texture.jpg");
  textureLinkSingleton->setLink(TextureType::MOB_MISSILE, "textures/missile_texture.jpg");
  textureLinkSingleton->setLink(TextureType::VILLAGER_SKIN, "textures/villager_skin.png");
  //textureLinkSingleton->setLink(TextureType::MOB_MISSILE, "textures/unnamed.png");
}

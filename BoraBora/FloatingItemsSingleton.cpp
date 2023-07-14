#pragma once

#include "FloatingItemsSingleton.h"
#include "Rectangle.h"
#include "TextureType.h"
#include <iostream>
#include "TextureAtlasSingleton.h"
#include <cassert>
#include "WorldBlocksSingleton.h"

using namespace std;

void FloatingItemsSingleton::loadVertexArray() {
  m_varray.resize(4 * (int)m_floatingItems.size());
  for (int i = 0; i < (int)m_floatingItems.size(); i++) {

    int column = m_floatingItems[i].second.first;
    int row = m_floatingItems[i].second.second;

    float eps = 0.2;

    BlockType blockType = m_floatingItems[i].first;
    TextureType type = TextureType::COUNT;

    if (blockType == BlockType::DIAMOND) type = TextureType::BLOCK_DIAMOND;
    if (blockType == BlockType::DIRT) type = TextureType::BLOCK_DIRT;
    if (blockType == BlockType::VOID) type = TextureType::BLOCK_VOID;

    assert(type != TextureType::COUNT);

    //cout << (type == TextureType::BLOCK_VOID);

    Rectangle textureRectangle = TextureAtlasSingleton::getInstance()->getTextureRectangle(type);

    sf::Vertex topLeft = sf::Vector2f(column + eps, row + eps);
    sf::Vertex topRight = sf::Vector2f(column + 1 - eps, row + eps);
    sf::Vertex bottomLeft = sf::Vector2f(column + eps, row + 1 - eps);
    sf::Vertex bottomRight = sf::Vector2f(column + 1 - eps, row + 1 - eps);

    topLeft.texCoords = { textureRectangle.getRowMin(), textureRectangle.getColumnMin() };
    topRight.texCoords = { textureRectangle.getRowMax(), textureRectangle.getColumnMin() };
    bottomRight.texCoords = { textureRectangle.getRowMax(), textureRectangle.getColumnMax() };
    bottomLeft.texCoords = { textureRectangle.getRowMin(), textureRectangle.getColumnMax() };

    topLeft.color = topRight.color = bottomRight.color = bottomLeft.color = sf::Color::Blue;

    //cout << " : " << textureRectangle.getRowMin() << "..." << textureRectangle.getRowMax() << " | " << textureRectangle.getColumnMin() << "..." << textureRectangle.getColumnMax() << "\n";

    m_varray[4 * i + 0] = topLeft;
    m_varray[4 * i + 1] = topRight;
    m_varray[4 * i + 2] = bottomRight;
    m_varray[4 * i + 3] = bottomLeft;

    for (int j = 4 * i; j <= 4 * i + 3; j++) {
      //swap(m_varray[j].texCoords.x, m_varray[j].texCoords.y);
    }
  }
}

void FloatingItemsSingleton::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const {
  renderStates.texture = &TextureAtlasSingleton::getInstance()->getTextureBand();
  renderTarget.draw(m_varray, &TextureAtlasSingleton::getInstance()->getTextureBand());
}


FloatingItemsSingleton* FloatingItemsSingleton::m_instance = nullptr;

FloatingItemsSingleton::FloatingItemsSingleton() : m_maxSize(0), m_varray(sf::Quads) {}

FloatingItemsSingleton* FloatingItemsSingleton::getInstance() {
  return m_instance;
}

void FloatingItemsSingleton::destroy() {
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

void FloatingItemsSingleton::createInstance() {
  if (!m_instance) {
    m_instance = new FloatingItemsSingleton();
  }
}

void FloatingItemsSingleton::set_max_size(int max_size) {
  m_maxSize = max_size;
}

void FloatingItemsSingleton::add(BlockType blockType, int column, int row) {
  m_floatingItems.push_back({ blockType, {column, row} });
}

void FloatingItemsSingleton::collect(PlayableMob& mob) {

  //cout << "aici : " << m_floatingItems.size() << "\n";
  for (int i = 0; i < (int)m_floatingItems.size(); i++) {

    int column = m_floatingItems[i].second.first;
    int row = m_floatingItems[i].second.second;

    Rectangle rectangle(row, row + 1, column, column + 1);

    if (doRectanglesIntersect(rectangle, mob.getBoundingBox())) {
      cout << "collected sir\n";

      swap(m_floatingItems[i], m_floatingItems.back());
      m_floatingItems.pop_back();
      i--;
    }

    // Add your logic here
  }
}

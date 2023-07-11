#pragma once

#include "WorldDrawerSingleton.h"
#include <iostream>
#include "GenericMob.h"
#include "PlayableMob.h"
#include "MissileMob.h"
#include "MobContainerSingleton.h"
#include <vector>

WorldDrawerSingleton* WorldDrawerSingleton::r_WorldDrawerSingleton = nullptr;

WorldDrawerSingleton::WorldDrawerSingleton(sf::RenderWindow& window)
  : m_window(window), m_vertexArray(sf::Quads) {}

void WorldDrawerSingleton::createInstance(sf::RenderWindow& window) {
  if (!r_WorldDrawerSingleton) {
    r_WorldDrawerSingleton = new WorldDrawerSingleton(window);
    r_WorldDrawerSingleton->m_camera.create(1000, 1000);
  }
}

WorldDrawerSingleton* WorldDrawerSingleton::getInstance(sf::RenderWindow& window) {
  if (!r_WorldDrawerSingleton) {
    r_WorldDrawerSingleton = new WorldDrawerSingleton(window);
  }
  return r_WorldDrawerSingleton;
}

void WorldDrawerSingleton::destroy() {
  if (r_WorldDrawerSingleton) {
    delete r_WorldDrawerSingleton;
    r_WorldDrawerSingleton = nullptr;
  }
}

void WorldDrawerSingleton::drawWorldOnWindow(Rectangle windowRectangle, Rectangle worldRectangle) {
  {
    m_camera.clear();
    int startColumn = static_cast<int>(worldRectangle.getColumnMin()) - 2;
    int endColumn = static_cast<int>(worldRectangle.getColumnMax()) + 2;
    int startRow = static_cast<int>(worldRectangle.getRowMin()) - 2;
    int endRow = static_cast<int>(worldRectangle.getRowMax()) + 2;

    m_vertexArray.resize(4 * (endRow - startRow) * (endColumn - startColumn));
    for (int i = 0; i < 4 * (endRow - startRow) * (endColumn - startColumn); i++) {
      m_vertexArray[i] = sf::Vertex();
    }

    int y = 0;
    for (int row = startRow; row < endRow; ++row) {
      for (int column = startColumn; column < endColumn; ++column) {

        if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
          y += 4;
          continue;
        }

        BlockType blockType = WorldBlocksSingleton::getInstance()->getBlockType(column, row);

        sf::Vertex topLeft = sf::Vector2f(column, row);
        sf::Vertex topRight = sf::Vector2f(column + 1, row);
        sf::Vertex bottomLeft = sf::Vector2f(column, row + 1);
        sf::Vertex bottomRight = sf::Vector2f(column + 1, row + 1);

        TextureType type = TextureType::COUNT;

        if (blockType == BlockType::DIAMOND) type = TextureType::BLOCK_DIAMOND;
        if (blockType == BlockType::DIRT) type = TextureType::BLOCK_DIRT;
        if (blockType == BlockType::VOID) type = TextureType::BLOCK_VOID;

        assert(type != TextureType::COUNT);

        Rectangle textureRectangle = TextureAtlasSingleton::getInstance()->getTextureRectangle(type);
        topLeft.texCoords = { textureRectangle.getRowMin(), textureRectangle.getColumnMin() };
        topRight.texCoords = { textureRectangle.getRowMax(), textureRectangle.getColumnMin() };
        bottomRight.texCoords = { textureRectangle.getRowMax(), textureRectangle.getColumnMax() };
        bottomLeft.texCoords = { textureRectangle.getRowMin(), textureRectangle.getColumnMax() };

        m_vertexArray[y] = topLeft;
        m_vertexArray[y + 1] = topRight;
        m_vertexArray[y + 2] = bottomRight;
        m_vertexArray[y + 3] = bottomLeft;

        y += 4;
      }
    }

    assert(y == m_vertexArray.getVertexCount());

    float xmin = worldRectangle.getColumnMin();
    float xmax = worldRectangle.getColumnMax();
    float ymin = worldRectangle.getRowMin();
    float ymax = worldRectangle.getRowMax();

    m_camera.setSize(sf::Vector2f(xmax - xmin, ymax - ymin));
    m_camera.setCenter((sf::Vector2f(xmin, ymin) + sf::Vector2f(xmax, ymax)) * 0.5f);
    m_camera.draw(m_vertexArray, &TextureAtlasSingleton::getInstance()->getTextureBand());

    for (int i = 0; i < MobContainerSingleton::getInstance()->getMobCount(); i++) {
      m_camera.draw(*MobContainerSingleton::getInstance()->getMob(i));
    }
    // windowRectangle = view port
    m_camera.drawCamera(windowRectangle, m_window);
  }

}

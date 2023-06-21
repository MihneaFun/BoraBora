#pragma once

#include "WorldDrawerSingleton.h"
#include <iostream>

WorldDrawerSingleton* WorldDrawerSingleton::r_WorldDrawerSingleton = nullptr;

WorldDrawerSingleton::WorldDrawerSingleton(sf::RenderWindow& window)
  : m_window(window), m_vertexArray(sf::Quads) {}

void WorldDrawerSingleton::createInstance(sf::RenderWindow& window) {
  if (!r_WorldDrawerSingleton) {
    r_WorldDrawerSingleton = new WorldDrawerSingleton(window);
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
  sf::Vector2u wantedSize = m_window.getSize();
  int d = 1000;
  wantedSize = sf::Vector2u(d, d);
  if (m_renderTexture.getSize() != wantedSize) {
    std::cout << "resize happened\n";
    m_renderTexture.create(wantedSize.x, wantedSize.y);
  }

  m_renderTexture.clear();

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

  sf::CircleShape shape;
  shape.setRadius(0.1);
  shape.setPosition(sf::Vector2f(0.5, 0.5));
  shape.setOrigin(sf::Vector2f(1, 1) * shape.getRadius());

  sf::View view;
  view.setSize(sf::Vector2f(endColumn - startColumn, endRow - startRow));
  view.setCenter((sf::Vector2f(startColumn, startRow) + sf::Vector2f(endColumn, endRow)) * 0.5f);
  m_renderTexture.setView(view);
  m_renderTexture.draw(m_vertexArray, &TextureAtlasSingleton::getInstance()->getTextureBand());
  m_renderTexture.display();

  // Create a VertexArray for displaying the RenderTexture.
  sf::VertexArray windowVertexArray(sf::Quads, 4);

  // Define quad vertices for full window.
  windowVertexArray[0].position = sf::Vector2f(windowRectangle.getColumnMin(), windowRectangle.getRowMin());
  windowVertexArray[1].position = sf::Vector2f(windowRectangle.getColumnMax(), windowRectangle.getRowMin());
  windowVertexArray[2].position = sf::Vector2f(windowRectangle.getColumnMax(), windowRectangle.getRowMax());
  windowVertexArray[3].position = sf::Vector2f(windowRectangle.getColumnMin(), windowRectangle.getRowMax());

  // Define texture coordinates to show whole texture.

  Rectangle oldBoundingBox(startRow, endRow, startColumn, endColumn);
  Rectangle newBoundingBox(0, m_renderTexture.getSize().y, 0, m_renderTexture.getSize().x);
  Rectangle newRect = getNewCoords(oldBoundingBox, newBoundingBox, worldRectangle);

  windowVertexArray[0].texCoords = sf::Vector2f(newRect.getColumnMin(), newRect.getRowMin());
  windowVertexArray[1].texCoords = sf::Vector2f(newRect.getColumnMax(), newRect.getRowMin());
  windowVertexArray[2].texCoords = sf::Vector2f(newRect.getColumnMax(), newRect.getRowMax());
  windowVertexArray[3].texCoords = sf::Vector2f(newRect.getColumnMin(), newRect.getRowMax());

  m_window.draw(windowVertexArray, &m_renderTexture.getTexture());
}

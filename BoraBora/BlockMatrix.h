#pragma once

#include "BlockType.h"
#include "TextureAtlasSingleton.h"
#include "Rectangle.h"
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include <vector>

class BlockMatrix : public sf::Drawable {
public:
  BlockMatrix(int columns, int rows);

  void setRectangle(Rectangle rectangle);
  void setBlockType(int column, int row, BlockType newBlockType);
  Rectangle getRectangle() const;
  BlockType getBlockType(int column, int row) const;
  void update(float dt);
  void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const;

private:
  float m_inner_clock;
  Rectangle getRectangle(int column, int row) const;

  sf::VertexArray m_vertexArray;
  int m_columns;
  int m_rows;
  int m_i;
  int m_j;
  Rectangle m_rectangle;
  std::vector<std::vector<BlockType>> m_blocks;
};

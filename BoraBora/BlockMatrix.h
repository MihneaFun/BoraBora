#pragma once

#include "BlockType.h"
#include "Rectangle.h"
#include <SFML/Graphics.hpp>
#include <vector>

class BlockMatrix : public sf::Drawable {
public:
  BlockMatrix(int columns, int rows);

  void setRectangle(Rectangle rectangle);
  void setBlockType(int column, int row, BlockType newBlockType);
  void setNumber(int column, int row, int number);
  int getNumber(int column, int row, int number) const;
  Rectangle getRectangle() const;
  BlockType getBlockType(int column, int row) const;
  void update(float dt);
  void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const;

  int getColumnsSize() const;
  int getRowsSize() const;

private:
  sf::Font m_font;
  float m_inner_clock;
  Rectangle getRectangle(int column, int row) const;
  sf::VertexArray m_vertexArray;
  int m_columns;
  int m_rows;
  int m_i;
  int m_j;
  Rectangle m_rectangle;
  std::vector<std::vector<BlockType>> m_blocks;
  std::vector<std::vector<int>> m_numbers;
};

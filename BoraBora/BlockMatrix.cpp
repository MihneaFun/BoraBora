#include "BlockMatrix.h"
#include "TextureAtlasSingleton.h"
#include <cassert>

BlockMatrix::BlockMatrix(int columns, int rows) : m_columns(columns), m_rows(rows), m_rectangle(0, 0, 0, 0), m_vertexArray(sf::Quads), m_i(0), m_j(0), m_inner_clock(10000) {
  m_blocks.resize(m_rows, std::vector<BlockType>(m_columns));
  assert(columns >= 1);
  assert(rows >= 1);
}

void BlockMatrix::setRectangle(Rectangle rectangle) {
  m_rectangle = rectangle;
}

void BlockMatrix::setBlockType(int column, int row, BlockType newBlockType) {
  assert(column >= 0 && column < m_columns);
  assert(row >= 0 && row < m_rows);

  m_blocks[row][column] = newBlockType;
}

Rectangle BlockMatrix::getRectangle() const {
  return m_rectangle;
}

BlockType BlockMatrix::getBlockType(int column, int row) const {
  assert(column >= 0 && column < m_columns);
  assert(row >= 0 && row < m_rows);

  return m_blocks[row][column];
}

void BlockMatrix::update(float dt) {
  m_inner_clock += dt;
  m_vertexArray.resize(4 * m_rows * m_columns);
  int y = 0;

  float dimColumn = (m_rectangle.getColumnMax() - m_rectangle.getColumnMin()) / m_columns;
  float dimRow = (m_rectangle.getRowMax() - m_rectangle.getRowMin()) / m_rows;



  for (int row = 0; row < m_rows; row++) {
    for (int column = 0; column < m_columns; column++) {
      BlockType blockType = m_blocks[row][column];

      Rectangle location = getRectangle(column, row);

      sf::Vertex topLeft = sf::Vector2f(location.getColumnMin(), location.getRowMin());
      sf::Vertex topRight = sf::Vector2f(location.getColumnMax(), location.getRowMin());
      sf::Vertex bottomLeft = sf::Vector2f(location.getColumnMin(), location.getRowMax());
      sf::Vertex bottomRight = sf::Vector2f(location.getColumnMax(), location.getRowMax());

      Rectangle textureRectangle = TextureAtlasSingleton::getInstance()->getTextureRectangle(blockType);
      topLeft.texCoords = { textureRectangle.getColumnMin(), textureRectangle.getRowMin() };
      topRight.texCoords = { textureRectangle.getColumnMax(), textureRectangle.getRowMin() };
      bottomLeft.texCoords = { textureRectangle.getColumnMin(), textureRectangle.getRowMax() };
      bottomRight.texCoords = { textureRectangle.getColumnMax(), textureRectangle.getRowMax() };

      m_vertexArray[y] = topLeft;
      m_vertexArray[y + 1] = topRight;
      m_vertexArray[y + 2] = bottomRight;
      m_vertexArray[y + 3] = bottomLeft;

      y += 4;
    }
  }
  if (sf::Joystick::isConnected(0) && m_inner_clock >= 0.2) {
    float position = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    if (position > 50) {
      m_j = (m_j + 1) % m_columns;
      m_inner_clock = 0;
    }
    if (position < -50) {
      m_j = (m_j + m_columns - 1) % m_columns;
      m_inner_clock = 0;
    }
  }
  if (sf::Joystick::isConnected(0) && m_inner_clock >= 0.2) {
    float position = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
    if (position > 50) {
      m_inner_clock = 0;
      m_i = (m_i + m_rows - 1) % m_rows;
    }
    if (position < -50) {
      m_i = (m_i + 1) % m_rows;
      m_inner_clock = 0;
    }
  }
}

void BlockMatrix::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const {
  renderStates.texture = &TextureAtlasSingleton::getInstance()->getTextureBand();
  renderTarget.draw(m_vertexArray, renderStates);

  sf::VertexArray frame(sf::Quads, 4);
  Rectangle location = getRectangle(m_j, m_i);

  sf::Vertex topLeft = sf::Vector2f(location.getColumnMin(), location.getRowMin());
  sf::Vertex topRight = sf::Vector2f(location.getColumnMax(), location.getRowMin());
  sf::Vertex bottomLeft = sf::Vector2f(location.getColumnMin(), location.getRowMax());
  sf::Vertex bottomRight = sf::Vector2f(location.getColumnMax(), location.getRowMax());

  Rectangle textureRectangle = TextureAtlasSingleton::getInstance()->getTextureRectangle(TextureType::FRAME);
  topLeft.texCoords = { textureRectangle.getColumnMin(), textureRectangle.getRowMin() };
  topRight.texCoords = { textureRectangle.getColumnMax(), textureRectangle.getRowMin() };
  bottomLeft.texCoords = { textureRectangle.getColumnMin(), textureRectangle.getRowMax() };
  bottomRight.texCoords = { textureRectangle.getColumnMax(), textureRectangle.getRowMax() };

  frame[0] = topLeft;
  frame[1] = topRight;
  frame[2] = bottomRight;
  frame[3] = bottomLeft;
  renderTarget.draw(frame, renderStates);
}

Rectangle BlockMatrix::getRectangle(int column, int row) const {
  float dimColumn = (m_rectangle.getColumnMax() - m_rectangle.getColumnMin()) / m_columns;
  float dimRow = (m_rectangle.getRowMax() - m_rectangle.getRowMin()) / m_rows;
  return Rectangle(m_rectangle.getColumnMin() + dimColumn * column, m_rectangle.getColumnMin() + dimColumn * (column + 1), m_rectangle.getRowMin() + dimRow * row, m_rectangle.getRowMin() + dimRow * (row + 1));
}

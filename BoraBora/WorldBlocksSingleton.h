#pragma once

#include <vector>
#include <cassert>
#include "BlockType.h"

class WorldBlocksSingleton {
private:
  std::vector<BlockType> m_blocks;
  int m_width, m_height;

  WorldBlocksSingleton(int width, int height);

  WorldBlocksSingleton(const WorldBlocksSingleton& other) = delete;
  WorldBlocksSingleton(WorldBlocksSingleton&& other) = delete;

  static WorldBlocksSingleton* r_WorldBlocksSingleton;
public:
  static WorldBlocksSingleton* getInstance();

  static void createInstance(int width, int height);

  static void destroy();

  BlockType getBlockType(int column, int row);

  void setBlockType(int column, int row, BlockType newBlockType);

  int getWidth() const;

  int getHeight() const;
};

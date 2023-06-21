#pragma once

#include "WorldBlocksSingleton.h"
#include <random>

WorldBlocksSingleton* WorldBlocksSingleton::r_WorldBlocksSingleton = nullptr;


WorldBlocksSingleton::WorldBlocksSingleton(int width, int height)
  : m_width(width), m_height(height), m_blocks(height* width, BlockType::VOID) {

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distr(10, 14); // define the range

  for (int column = 0; column < width; column++) {
    if (column <= 7) {
      for (int row = 0; row < height; row++) {
        if (row >= 2 && row <= 20) {
          continue;
        }
        if ((row + column) % 2 == 0) {
          setBlockType(column, row, BlockType::DIAMOND);
        }
        else {
          setBlockType(column, row, BlockType::DIRT);
        }
      }
      continue;
    }
    
    int randomHeight = distr(gen);  // generate random height
    for (int row = 0; row < randomHeight; row++) {
      if (column <= 20 && row >= 2 && row <= 3) {
        continue;
      }
      if (column <= 25 && row >= 2 && row <= 2) {
        continue;
      }
      if ((row + column) % 2 == 0) {
        setBlockType(column, row, BlockType::DIAMOND);
      }
      else {
        setBlockType(column, row, BlockType::DIRT);
      }
    }
  }
}


WorldBlocksSingleton* WorldBlocksSingleton::getInstance() {
  return r_WorldBlocksSingleton;
}

void WorldBlocksSingleton::createInstance(int width, int height) {
  if (!r_WorldBlocksSingleton) {
    r_WorldBlocksSingleton = new WorldBlocksSingleton(width, height);
  }
}

void WorldBlocksSingleton::destroy() {
  if (r_WorldBlocksSingleton) {
    delete r_WorldBlocksSingleton;
    r_WorldBlocksSingleton = nullptr;
  }
}

BlockType WorldBlocksSingleton::getBlockType(int column, int row) {
  assert(column >= 0 && column < m_width&& row >= 0 && row < m_height);
  return m_blocks[row * m_width + column];
}

void WorldBlocksSingleton::setBlockType(int column, int row, BlockType newBlockType) {
  assert(column >= 0 && column < m_width&& row >= 0 && row < m_height);
  m_blocks[row * m_width + column] = newBlockType;
}

int WorldBlocksSingleton::getWidth() const {
  return m_width;
}

int WorldBlocksSingleton::getHeight() const {
  return m_height;
}

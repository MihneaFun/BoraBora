#pragma once

#include "GenericMob.h"
#include "WorldBlocksSingleton.h"

#include <iostream>
#include <iomanip>

GenericMob::GenericMob() {
}

#include <iostream>

using namespace std;

const float Epsilon = 0.000001;

bool GenericMob::Xupdate(float dt_init) {
  if (getVelocity().x > 0) {
    float dt = dt_init;
    Rectangle rect = getBoundingBox();
    int minRow = floor(rect.getRowMin());
    int maxRow = ceil(rect.getRowMax()) - 1;
    int nextColumn = ceil(rect.getColumnMax());
    if (0 <= nextColumn && nextColumn < WorldBlocksSingleton::getInstance()->getWidth()) {
      minRow = max(minRow, 0);
      maxRow = min(maxRow, WorldBlocksSingleton::getInstance()->getHeight() - 1);
      for (int row = minRow; row <= maxRow; row++) {
        int column = nextColumn;
        assert(!(column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()));
        if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
          continue;
        }
        dt = min(dt, (nextColumn - rect.getColumnMax()) / getVelocity().x);
      }
    }
    dt = max(dt, 0.0f);
    justElapseDtX(dt);
    return dt < dt_init;
  }
  if (getVelocity().x < 0) {
    float dt = dt_init;
    Rectangle rect = getBoundingBox();
    int minRow = floor(rect.getRowMin());
    int maxRow = ceil(rect.getRowMax()) - 1;
    int previousColumn = floor(rect.getColumnMin() + Epsilon) - 1;
    if (0 <= previousColumn && previousColumn < WorldBlocksSingleton::getInstance()->getWidth()) {
      minRow = max(minRow, 0);
      maxRow = min(maxRow, WorldBlocksSingleton::getInstance()->getHeight() - 1);
      for (int row = minRow; row <= maxRow; row++) {
        int column = previousColumn;
        assert(!(column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()));
        if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
          continue;
        }
        dt = min(dt, (rect.getColumnMin() - (previousColumn + 1)) / (-getVelocity().x));
      }
    }
    dt = max(dt, 0.0f);
    justElapseDtX(dt);
    return dt < dt_init;
  }
  justElapseDtX(dt_init);
  return false;
}

#include "WorldDrawerSingleton.h"

bool GenericMob::Yupdate(float dt_init) {
  if (is()) {
    //cout << " : " << getVelocity().y << "\n";
  }
  if (getVelocity().y > 0) {
    float dt = dt_init;
    Rectangle rect = getBoundingBox();
    int minColumn = floor(rect.getColumnMin() + Epsilon);
    int maxColumn = ceil(rect.getColumnMax()) - 1;
    int nextRow = ceil(rect.getRowMax());
    if (0 <= nextRow && nextRow < WorldBlocksSingleton::getInstance()->getHeight()) {
      minColumn = max(minColumn, 0);
      maxColumn = min(maxColumn, WorldBlocksSingleton::getInstance()->getWidth() - 1);
      for (int column = minColumn; column <= maxColumn; column++) {
        int row = nextRow;
        assert(!(column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()));

        if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
          continue;
        }
        dt = min(dt, (nextRow - rect.getRowMax()) / getVelocity().y);
      }
    }
    dt = max(dt, 0.0f);
    justElapseDtY(dt);
    return dt < dt_init;
  }
  if (getVelocity().y < 0) {
    float dt = dt_init;
    Rectangle rect = getBoundingBox();
    int minColumn = floor(rect.getColumnMin() + Epsilon);
    int maxColumn = ceil(rect.getColumnMax()) - 1;
    int previousRow = floor(rect.getRowMin() + Epsilon) - 1;
    if (0 <= previousRow && previousRow < WorldBlocksSingleton::getInstance()->getHeight()) {
      minColumn = max(minColumn, 0);
      maxColumn = min(maxColumn, WorldBlocksSingleton::getInstance()->getWidth() - 1);
      for (int column = minColumn; column <= maxColumn; column++) {
        int row = previousRow;
        
        assert(!(column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()));
        if (is()) {
          //cout << "lmao\n";
          WorldBlocksSingleton::getInstance()->all.insert(make_pair(column, row));
        }
        if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
          continue;
        }
        //cout << "aici\n";
        dt = min(dt, (rect.getRowMin() - (previousRow + 1)) / (-getVelocity().y));
      }
    }
    dt = max(dt, 0.0f);
    justElapseDtY(dt);
    return dt < dt_init;
  }
  justElapseDtY(dt_init);
  return false;
}

bool GenericMob::physicsUpdate(float dt) {
  const float EPSILON = 0;
  addForce(sf::Vector2f(-getVelocity().x, 0) * 0.004f);
  addForce(sf::Vector2f(0, -0.1));
  applyForces();
  clearForces();
  bool isX = Xupdate(dt);
  bool isY = Yupdate(dt);
  if (isX) {
    setVelocity(sf::Vector2f(0, getVelocity().y));
  }
  if (isY) {
    setVelocity(sf::Vector2f(getVelocity().x, 0));
  }
  return isX || isY;
}
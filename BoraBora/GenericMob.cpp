#pragma once

#include "GenericMob.h"
#include "WorldBlocksSingleton.h"

GenericMob::GenericMob() {
}

bool GenericMob::physicsUpdate(float dt) {
  applyForces();
  clearForces();
  bool invalid = false;
  float curdt = dt;
  bool touch = false;
  for (int iter = 1; iter <= 30, curdt *= 0.5f; iter++) {
    Rectangle rect = getBoundingBoxAfterUpdateX(curdt);
    int startRow = rect.getRowMin() - 2, endRow = rect.getRowMax() + 2;
    int startColumn = rect.getColumnMin() - 2, endColumn = rect.getColumnMax() + 2;
    bool valid = true;
    for (int row = startRow; row <= endRow && valid; ++row) {
      for (int column = startColumn; column <= endColumn && valid; ++column) {
        if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
          continue;
        }

        if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
          continue;
        }

        BlockType blockType = WorldBlocksSingleton::getInstance()->getBlockType(column, row);

        Rectangle thisRectangle(row, row + 1, column, column + 1);

        if (doRectanglesIntersect(thisRectangle, rect)) {
          valid = false;
        }
      }
    }
    if (iter == 1 && !valid) {
      touch = true;
    }
    if (valid) {
      justElapseDtX(curdt);
    }
    else {
      invalid = true;
    }
  }
  if (invalid) {
    setVelocity(sf::Vector2f(0, getVelocity().y));
  }
  {
    bool invalid = false;
    float curdt = dt;
    for (int iter = 1; iter <= 30, curdt *= 0.5f; iter++) {
      Rectangle rect = getBoundingBoxAfterUpdateY(curdt);
      int startRow = rect.getRowMin() - 2, endRow = rect.getRowMax() + 2;
      int startColumn = rect.getColumnMin() - 2, endColumn = rect.getColumnMax() + 2;
      bool valid = true;
      for (int row = startRow; row <= endRow && valid; ++row) {
        for (int column = startColumn; column <= endColumn && valid; ++column) {

          if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
            continue;
          }

          if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
            continue;
          }

          BlockType blockType = WorldBlocksSingleton::getInstance()->getBlockType(column, row);

          Rectangle thisRectangle(row, row + 1, column, column + 1);

          if (doRectanglesIntersect(thisRectangle, rect)) {
            valid = false;
          }
        }
      }
      if (iter == 1 && !valid) {
        touch = true;
      }
      if (valid) {
       justElapseDtY(curdt);
      }
      else {
        invalid = true;
      }
    }
    if (invalid) {
      setVelocity(sf::Vector2f(getVelocity().x, 0));
    }
  }
  return touch;
}
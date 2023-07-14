#pragma once

#include "GenericMob.h"
#include "WorldBlocksSingleton.h"

#include <iostream>
#include <iomanip>
#include "FloatingPointFunctions.h"

GenericMob::GenericMob() {
}

#include <iostream>

using namespace std;

const float Epsilon = 0.000001;

bool GenericMob::Xupdate(float dt_init) {
  //cout << setprecision(6) << fixed << getBoundingBox().getColumnMin() << " " << floor(getBoundingBox().getColumnMin() + Epsilon) << "\n";
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

bool GenericMob::Yupdate(float dt_init) {
  //WorldBlocksSingleton::getInstance()->spec.clear();
  //cout << setprecision(6) << fixed << getBoundingBox().getColumnMin() << " " << floor(getBoundingBox().getColumnMin() + Epsilon) << "\n";
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
    int previousRow = floor(rect.getRowMin()) - 1;
    if (0 <= previousRow && previousRow < WorldBlocksSingleton::getInstance()->getHeight()) {
      minColumn = max(minColumn, 0);
      maxColumn = min(maxColumn, WorldBlocksSingleton::getInstance()->getWidth() - 1);
      for (int column = minColumn; column <= maxColumn; column++) {
        int row = previousRow;
        assert(!(column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()));

        WorldBlocksSingleton::getInstance()->spec.insert({ column, row });
        if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
          continue;
        }
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
  //addForce(sf::Vector2f(-getVelocity().x, 0) * 0.04f);
  addForce(sf::Vector2f(0, -0.1));
  //addForce(sf::Vector2f(0, -1));
  applyForces();
  clearForces();


  {
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

  bool invalid = false;
  bool touchx = false, touchy = false;
  {
    if (abs(getVelocity().x) > EPSILON) {
      Rectangle rect = getBoundingBox();
      int startRow = rect.getRowMin() - 2, endRow = rect.getRowMax() + 2;
      int startColumn = rect.getColumnMin() - 2, endColumn = rect.getColumnMax() + 2;
      sf::Vector2f velocity = getVelocity();
      float curdt = dt;
      for (int row = startRow; row <= endRow; ++row) {
        for (int column = startColumn; column <= endColumn; ++column) {
          if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
            continue;
          }

          if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
            continue;
          }

          Rectangle thisRectangle(row, row + 1, column, column + 1);

          if (rect.getRowMax() < thisRectangle.getRowMin()) {
            continue;
          }

          if (thisRectangle.getRowMax() < rect.getRowMin()) {
            continue;
          }

          if (velocity.x > 0) {
            if (rect.getColumnMax() <= thisRectangle.getColumnMin()) {
              if ((thisRectangle.getColumnMin() - rect.getColumnMax()) / velocity.x < dt) {
                touchx = true;
                curdt = std::min(curdt, (thisRectangle.getColumnMin() - rect.getColumnMax()) / velocity.x);
              }
            }
          }
          if (velocity.x < 0) {
            // velocity.x < 0
            if (thisRectangle.getColumnMax() <= rect.getColumnMin()) {
              if ((rect.getColumnMin() - thisRectangle.getColumnMax()) / (-velocity.x) < dt) {
                touchx = true;
                curdt = std::min(curdt, (rect.getColumnMin() - thisRectangle.getColumnMax()) / (-velocity.x));
              }
            }
          }
        }
      }
      justElapseDtX(curdt);
      if (touchx) {
        setVelocity(sf::Vector2f(0, getVelocity().y));
      }
    }
    if (abs(getVelocity().y) > EPSILON) {
      Rectangle rect = getBoundingBox();
      int startRow = rect.getRowMin() - 2, endRow = rect.getRowMax() + 2;
      int startColumn = rect.getColumnMin() - 2, endColumn = rect.getColumnMax() + 2;
      sf::Vector2f velocity = getVelocity();
      float curdt = dt;
      for (int row = startRow; row <= endRow; ++row) {
        for (int column = startColumn; column <= endColumn; ++column) {
          if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
            continue;
          }

          if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
            continue;
          }

          Rectangle thisRectangle(row, row + 1, column, column + 1);

          if (rect.getColumnMax() < thisRectangle.getColumnMin()) {
            continue;
          }

          if (thisRectangle.getColumnMax() < rect.getColumnMin()) {
            continue;
          }

          if (velocity.y > 0) {
            if (rect.getRowMax() <= thisRectangle.getRowMin()) {
              if ((thisRectangle.getRowMin() - rect.getRowMax()) / velocity.y < dt) {
                curdt = std::min(curdt, (thisRectangle.getRowMin() - rect.getRowMax()) / velocity.y);
                touchy = true;
              }
            }
          }
          if (velocity.y < 0) {
            // velocity.y < 0
            if (thisRectangle.getRowMax() < rect.getRowMin()) {
              if ((rect.getRowMin() - thisRectangle.getRowMax()) / (-velocity.y) < dt) {
                curdt = std::min(curdt, (rect.getRowMin() - thisRectangle.getRowMax()) / (-velocity.y));
                touchy = true;
              }
            }
          }
        }
      }
      if (touchy) {
        std::cout << "touch y\n";
        setVelocity(sf::Vector2f(getVelocity().x, 0));
      }
      justElapseDtY(curdt);
    }
    return touchx || touchy;
  }
  bool touch = false;
  float curdt = dt;
  {
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

          WorldBlocksSingleton::getInstance()->nr2++;
          if (doRectanglesIntersect(thisRectangle, rect)) {
            valid = false;
          }
        }
      }
      if (iter == 1) {
        // break
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
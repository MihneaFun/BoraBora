#pragma once

#include "Rectangle.h"
#include "WorldBlocksSingleton.h"

bool doesRectangleIntersectNonVoidBlocks(const Rectangle& rect) {
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

      Rectangle thisRectangle(row, row + 1, column, column + 1);

      if (doRectanglesIntersect(thisRectangle, rect)) {
        return true;
      }
    }
  }
  return false;
}

Rectangle::Rectangle(float rowmin, float rowmax, float colmin, float colmax)
  : m_rowmin(rowmin), m_rowmax(rowmax), m_colmin(colmin), m_colmax(colmax) {}


float Rectangle::getRowMin() const {
  return m_rowmin;
}

float Rectangle::getRowMax() const {
  return m_rowmax;
}

float Rectangle::getColumnMin() const {
  return m_colmin;
}

float Rectangle::getColumnMax() const {
  return m_colmax;
}



bool doRectanglesIntersect(const Rectangle& r1, const Rectangle& r2) {
  WorldBlocksSingleton::getInstance()->nr++;
  // Condition for non-overlapping rectangles.
  if (r1.getRowMin() > r2.getRowMax() || r2.getRowMin() > r1.getRowMax() ||
    r1.getColumnMin() > r2.getColumnMax() || r2.getColumnMin() > r1.getColumnMax()) {
    return false;
  }
  return true;
}

sf::Vector2f getNewCoords(Rectangle oldView, Rectangle newView, sf::Vector2f position) {
  // Calculate relative position within the old view.
  float oldRelX = (position.x - oldView.getColumnMin()) / (oldView.getColumnMax() - oldView.getColumnMin());
  float oldRelY = (position.y - oldView.getRowMin()) / (oldView.getRowMax() - oldView.getRowMin());

  // Apply the relative position to the new view.
  float newX = newView.getColumnMin() + oldRelX * (newView.getColumnMax() - newView.getColumnMin());
  float newY = newView.getRowMin() + oldRelY * (newView.getRowMax() - newView.getRowMin());

  return sf::Vector2f(newX, newY);
}

Rectangle getNewCoords(Rectangle oldView, Rectangle newView, Rectangle rect) {
  // Get the four corners of the rectangle
  sf::Vector2f topLeft(rect.getColumnMin(), rect.getRowMin());
  sf::Vector2f topRight(rect.getColumnMax(), rect.getRowMin());
  sf::Vector2f bottomLeft(rect.getColumnMin(), rect.getRowMax());
  sf::Vector2f bottomRight(rect.getColumnMax(), rect.getRowMax());

  // Transform each corner using the getNewCoords function
  topLeft = getNewCoords(oldView, newView, topLeft);
  topRight = getNewCoords(oldView, newView, topRight);
  bottomLeft = getNewCoords(oldView, newView, bottomLeft);
  bottomRight = getNewCoords(oldView, newView, bottomRight);

  // Determine the new min and max column and row values
  float newColumnMin = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
  float newRowMin = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));

  float newColumnMax = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
  float newRowMax = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));

  // Return a new Rectangle using these new bounds
  return Rectangle(newRowMin, newRowMax, newColumnMin, newColumnMax);
}

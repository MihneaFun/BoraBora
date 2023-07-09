#pragma once
#include <SFML/Graphics.hpp>

class Rectangle {
public:
  Rectangle(float rowmin, float rowmax, float columnmin, float columnmax);
  float getRowMin() const;
  float getRowMax() const;
  float getColumnMin() const;
  float getColumnMax() const;

private:
  float m_rowmin;
  float m_rowmax;
  float m_colmin;
  float m_colmax;
};

bool doesRectangleIntersectNonVoidBlocks(const Rectangle &rectangle);

bool doRectanglesIntersect(const Rectangle& r1, const Rectangle& r2);

sf::Vector2f getNewCoords(Rectangle oldView, Rectangle newView, sf::Vector2f position);

Rectangle getNewCoords(Rectangle oldView, Rectangle newView, Rectangle rect);
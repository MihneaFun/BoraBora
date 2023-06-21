#pragma once

#include <SFML/System/Vector2.hpp>

class GenericMob {
public:
  GenericMob();

  virtual void update(float dt) = 0;
  virtual void addForce(const sf::Vector2f& force) = 0;
  virtual void applyForces() = 0;
  virtual void teleport(float column, float row) = 0;
  virtual void clearForces() = 0;

};

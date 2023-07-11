#pragma once

#include <SFML/System/Vector2.hpp>
#include "Rectangle.h"

class GenericMob : public sf::Drawable {
protected:
  bool physicsUpdate(float dt); // returns true if the mob touches a world block and false if it doesn't 

public:
  GenericMob();

  virtual bool requestDelete() = 0;
  virtual void update(float dt, std::vector<std::unique_ptr<GenericMob>>& mobs) = 0;
  virtual void addForce(const sf::Vector2f& force) = 0;
  virtual void applyForces() = 0;
  virtual void teleport(float column, float row) = 0;
  virtual void clearForces() = 0;
  virtual Rectangle getBoundingBox() const = 0;
  virtual Rectangle getBoundingBoxAfterUpdateX(float dt) const = 0;
  virtual Rectangle getBoundingBoxAfterUpdateY(float dt) const = 0;
  virtual sf::Vector2f getVelocity() const = 0;
  virtual void justElapseDtX(float dt) = 0;
  virtual void justElapseDtY(float dt) = 0;
  virtual void setVelocity(sf::Vector2f velocity) = 0;
  virtual sf::Vector2f getCenter() const = 0;
};

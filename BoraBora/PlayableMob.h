#pragma once

#include "GenericMob.h"
#include "Rectangle.h"
#include <SFML/System/Vector2.hpp>

class PlayableMob : public GenericMob {
public:
  PlayableMob();

  void update(float dt) override;
  void addForce(const sf::Vector2f& force) override;
  void applyForces() override;
  void justElapseDtX(float dt);
  void justElapseDtY(float dt);
  void teleport(float column, float row)  override;
  void clearForces() override;

  void setMass(float mass);
  void setVelocity(sf::Vector2f velocity);
  float getMass() const;

  Rectangle getBoundingBox() const;
  Rectangle getBoundingBoxAfterUpdateX(float dt) const;
  Rectangle getBoundingBoxAfterUpdateY(float dt) const;

  float getRow() const;
  float getColumn() const;

  sf::Vector2f getVelocity() const;

private:
  float m_column;
  float m_row;
  float m_mass;

  sf::Vector2f m_totalForce;
  sf::Vector2f m_velocity;
};

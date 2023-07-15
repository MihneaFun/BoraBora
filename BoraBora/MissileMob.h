#pragma once

#include "GenericMob.h"
#include "Rectangle.h"
#include <SFML/System/Vector2.hpp>

class MissileMob : public GenericMob {
public:
  MissileMob();


  bool requestDelete() override;
  void ignite();
  void addForce(const sf::Vector2f& force) override;
  void applyForces() override;
  void justElapseDtX(float dt) override;
  void justElapseDtY(float dt) override;
  void teleport(float column, float row)  override;
  void clearForces() override;

  void setMass(float mass);
  void setVelocity(sf::Vector2f velocity) override;
  float getMass() const;

  Rectangle getBoundingBox() const override;
  Rectangle getBoundingBoxAfterUpdateX(float dt) const override;
  Rectangle getBoundingBoxAfterUpdateY(float dt) const override;

  float getRow() const;
  float getColumn() const;
  
  sf::Vector2f getVelocity() const override;

  Rectangle getBoundingBoxGivenPosition(float row, float column) const;

  sf::Vector2f getCenter() const override;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  
  void update(float dt) override;

  bool isExploding() const;

  void killFromWorld() override;

private:

  bool is() override {
    return 0;
  }

  float m_remainingIgnitingTime;
  bool m_igniting;
  float m_column;
  float m_row;
  float m_mass;

  sf::Vector2f m_totalForce;
  sf::Vector2f m_velocity;
};

#pragma once

#include "FloatingBlockMob.h"
#include "TextureAtlasSingleton.h"
#include "TextureType.h"
#include <iostream>
#include "FloatingItemsSingleton.h"
#include "WorldBlocksSingleton.h"

FloatingBlockMob::FloatingBlockMob() : m_mass(1.0f) {}

void FloatingBlockMob::killFromWorld() {
}

bool FloatingBlockMob::requestDelete() {
  //return isExploding();
  return false;
}

void FloatingBlockMob::update(float dt) {
  bool touch = physicsUpdate(dt);
}

void FloatingBlockMob::justElapseDtX(float dt) {
  // Move the mob according to its velocity.
  m_column += m_velocity.x * dt;
}

void FloatingBlockMob::justElapseDtY(float dt) {
  // Move the mob according to its velocity.
  m_row += m_velocity.y * dt;
}

void FloatingBlockMob::addForce(const sf::Vector2f& force) {
  m_totalForce += force;
}

void FloatingBlockMob::applyForces() {
  // no physics on floating blocks! (for now)
  return;
  // F = m*a, therefore a = F/m. Multiply by dt to get the change in velocity.
  sf::Vector2f acceleration = m_totalForce / m_mass;
  m_velocity += acceleration;

}

void FloatingBlockMob::teleport(float column, float row) {
  m_column = column;
  m_row = row;
  // We also clear the velocity when teleporting, assuming that teleportation doesn't preserve momentum.
  m_velocity = sf::Vector2f(0, 0);
}

void FloatingBlockMob::clearForces() {
  m_totalForce = sf::Vector2f(0, 0);
}

void FloatingBlockMob::setVelocity(sf::Vector2f velocity) {
  m_velocity = velocity;
}

void FloatingBlockMob::setMass(float mass) {
  m_mass = mass;
}

float FloatingBlockMob::getMass() const {
  return m_mass;
}

sf::Vector2f FloatingBlockMob::getVelocity() const {
  return m_velocity;
}

Rectangle FloatingBlockMob::getBoundingBoxGivenPosition(float row, float column) const {
  float rowMin = row - 0.4f;
  float rowMax = row + 0.4f;
  float colMin = column - 0.4f;
  float colMax = column + 0.4f;

  return Rectangle(rowMin, rowMax, colMin, colMax);
}

Rectangle FloatingBlockMob::getBoundingBox() const {
  return getBoundingBoxGivenPosition(m_row, m_column);
}

Rectangle FloatingBlockMob::getBoundingBoxAfterUpdateX(float dt) const {
  return getBoundingBoxGivenPosition(m_row, m_column + m_velocity.x * dt);
}

Rectangle FloatingBlockMob::getBoundingBoxAfterUpdateY(float dt) const {
  return getBoundingBoxGivenPosition(m_row + m_velocity.y * dt, m_column);
}

sf::Vector2f FloatingBlockMob::getCenter() const {
  return sf::Vector2f(m_column, m_row);
}

float FloatingBlockMob::getRow() const {
  return m_row;
}

float FloatingBlockMob::getColumn() const {
  return m_column;
}

void FloatingBlockMob::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const {
  Rectangle pos = getBoundingBox();
  sf::VertexArray varray(sf::Quads, 4);
  varray[0].position = sf::Vector2f(pos.getColumnMin(), pos.getRowMin());
  varray[1].position = sf::Vector2f(pos.getColumnMin(), pos.getRowMax());
  varray[2].position = sf::Vector2f(pos.getColumnMax(), pos.getRowMax());
  varray[3].position = sf::Vector2f(pos.getColumnMax(), pos.getRowMin());

  Rectangle rect = TextureAtlasSingleton::getInstance()->getTextureRectangle(TextureType::MOB_MISSILE);
  varray[0].texCoords = sf::Vector2f(rect.getColumnMin(), rect.getRowMin());
  varray[1].texCoords = sf::Vector2f(rect.getColumnMin(), rect.getRowMax());
  varray[2].texCoords = sf::Vector2f(rect.getColumnMax(), rect.getRowMax());
  varray[3].texCoords = sf::Vector2f(rect.getColumnMax(), rect.getRowMin());

  for (int i = 0; i < 4; i++) {
    std::swap(varray[i].texCoords.x, varray[i].texCoords.y);
  }

  renderStates.texture = &TextureAtlasSingleton::getInstance()->getTextureBand();
  renderTarget.draw(varray, renderStates);
}
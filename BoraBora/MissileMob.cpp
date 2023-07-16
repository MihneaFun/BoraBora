#pragma once

#include "MissileMob.h"
#include "TextureAtlasSingleton.h"
#include "TextureType.h"
#include <iostream>
#include "WorldBlocksSingleton.h"
#include "MobContainerSingleton.h"
#include "FloatingBlockMob.h"

MissileMob::MissileMob() : m_mass(1.0f), m_igniting(false), m_remainingIgnitingTime(2) {}

void MissileMob::killFromWorld() {
  Rectangle bb = getBoundingBox();
  for (int row = bb.getRowMin() - 2; row <= bb.getRowMax() + 2; row++) {
    for (int column = bb.getColumnMin() - 2; column <= bb.getColumnMin() + 2; column++) {
      if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
        continue;
      }
      if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
        continue;
      }
      std::unique_ptr<FloatingBlockMob> ptr = std::make_unique<FloatingBlockMob>();
      ptr->teleport(column + 0.5, row + 0.5);
      ptr->setVelocity(sf::Vector2f(0, 0));
      ptr->setBlockType(WorldBlocksSingleton::getInstance()->getBlockType(column, row));
      MobContainerSingleton::getInstance()->addMob(std::move(ptr));
      WorldBlocksSingleton::getInstance()->setBlockType(column, row, BlockType::VOID);
    }
  }
}


bool MissileMob::requestDelete() {
  return isExploding() || getBoundingBox().getRowMax() < -100;
}

void MissileMob::update(float dt) {
  bool touch = physicsUpdate(dt);
  if (touch) {
    ignite();
  }
  m_remainingIgnitingTime -= m_igniting * dt;
}

void MissileMob::justElapseDtX(float dt) {
  // Move the mob according to its velocity.
  m_column += m_velocity.x * dt;
}

void MissileMob::justElapseDtY(float dt) {
  // Move the mob according to its velocity.
  m_row += m_velocity.y * dt;
}

void MissileMob::addForce(const sf::Vector2f& force) {
  m_totalForce += force;
}

void MissileMob::applyForces() {
  // F = m*a, therefore a = F/m. Multiply by dt to get the change in velocity.
  sf::Vector2f acceleration = m_totalForce / m_mass;
  m_velocity += acceleration;
}

void MissileMob::teleport(float column, float row) {
  m_column = column;
  m_row = row;
  // We also clear the velocity when teleporting, assuming that teleportation doesn't preserve momentum.
  m_velocity = sf::Vector2f(0, 0);
}

void MissileMob::clearForces() {
  m_totalForce = sf::Vector2f(0, 0);
}

void MissileMob::setVelocity(sf::Vector2f velocity) {
  m_velocity = velocity;
}

void MissileMob::setMass(float mass) {
  m_mass = mass;
}

float MissileMob::getMass() const {
  return m_mass;
}

sf::Vector2f MissileMob::getVelocity() const {
  return m_velocity;
}

Rectangle MissileMob::getBoundingBoxGivenPosition(float row, float column) const {
  float rowMin = row - 0.7f;
  float rowMax = row + 0.7f;
  float colMin = column - 0.7f;
  float colMax = column + 0.7f;

  return Rectangle(colMin, colMax, rowMin, rowMax);
}

Rectangle MissileMob::getBoundingBox() const {
  return getBoundingBoxGivenPosition(m_row, m_column);
}

Rectangle MissileMob::getBoundingBoxAfterUpdateX(float dt) const {
  return getBoundingBoxGivenPosition(m_row, m_column + m_velocity.x * dt);
}

Rectangle MissileMob::getBoundingBoxAfterUpdateY(float dt) const {
  return getBoundingBoxGivenPosition(m_row + m_velocity.y * dt, m_column);
}

sf::Vector2f MissileMob::getCenter() const {
  return sf::Vector2f(m_column, m_row);
}

void MissileMob::ignite() {
  m_igniting = true;
}


float MissileMob::getRow() const {
  return m_row;
}

float MissileMob::getColumn() const {
  return m_column;
}

bool MissileMob::isExploding() const {
  return m_remainingIgnitingTime < 0;
}

void MissileMob::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const {
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

  if (m_igniting) {
    for (int i = 0; i < 4; i++) {
      float val = sin(m_remainingIgnitingTime * 20);
      val += 1;
      val /= 2;
      val *= 256;
      varray[i].color.r = val;
      varray[i].color.g = val;
      varray[i].color.b = val;
    }
  }

  renderStates.texture = &TextureAtlasSingleton::getInstance()->getTextureBand();
  renderTarget.draw(varray, renderStates);
}
#pragma once

#include "PlayableMob.h"
#include "KeyboardAndMouseSingleton.h"
#include "MissileMob.h"
#include <iostream>

PlayableMob::PlayableMob() : m_mass(1.0f), m_was(0), m_wasB(0), m_wasX(0) {}

bool PlayableMob::requestDelete() {
  return false;
}

void PlayableMob::update(float dt, std::vector<std::unique_ptr<GenericMob>>& mobs) {
  if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::Space))) {
    addForce(sf::Vector2f(0, 10));
  }

  bool is = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0));
  bool isB = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1));
  bool isX = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 2));
  if (!m_was && is) {
    addForce(sf::Vector2f(0, 10));
  }
  if (!m_wasB && isB) {
    mobs.push_back(std::make_unique<MissileMob>());
    sf::Vector2f loc = mobs[0]->getCenter();
    mobs.back()->teleport(loc.x, loc.y);
    mobs.back()->setVelocity(sf::Vector2f(20, 0));
    if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
      
      mobs.back().reset();
      mobs.pop_back();
    }
  }
  if (!m_wasX && isX) {
    mobs.push_back(std::make_unique<MissileMob>());
    sf::Vector2f loc = mobs[0]->getCenter();
    mobs.back()->teleport(loc.x, loc.y);
    mobs.back()->setVelocity(sf::Vector2f(-20, 0));
    if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
      mobs.back().reset();
      mobs.pop_back();
    }
  }
  m_was = is;
  m_wasB = isB;
  m_wasX = isX;
  if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::L))) {
    mobs.push_back(std::make_unique<MissileMob>());
    sf::Vector2f loc = mobs[0]->getCenter();
    mobs.back()->teleport(loc.x, loc.y);
    mobs.back()->setVelocity(sf::Vector2f(20, 0));
    if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
      mobs.back().reset();
      mobs.pop_back();
    }
  }
  if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::J))) {
    mobs.push_back(std::make_unique<MissileMob>());
    sf::Vector2f loc = mobs[0]->getCenter();
    mobs.back()->teleport(loc.x, loc.y);
    mobs.back()->setVelocity(sf::Vector2f(-20, 0));
    if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
      mobs.back().reset();
      mobs.pop_back();
    }
  }

  // Handle mob movement
  sf::Vector2f force(0.0f, 0.0f);

  if (KeyboardAndMouseSingleton::getInstance()->isKeyPressed((sf::Keyboard::A))) {
    force.x -= 1;
  }
  if (KeyboardAndMouseSingleton::getInstance()->isKeyPressed((sf::Keyboard::D))) {
    force.x += 1;
  }

  if (sf::Joystick::isConnected(0)) {
    float position = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    if (position > 50) {
      force.x += 1;
    }
    if (position < -50) {
      force.x -= 1;
    }
    //std::cout << "X-axis position: " << position << std::endl;
  }

  force *= 10.0f;

  // Scale the force by dt to make the movement time-dependent
  force = force * dt;

  // Apply the calculated force to the mob
  addForce(force);

  physicsUpdate(dt);
}

void PlayableMob::justElapseDtX(float dt) {
  // Move the mob according to its velocity.
  m_column += m_velocity.x * dt;
}

void PlayableMob::justElapseDtY(float dt) {
  // Move the mob according to its velocity.
  m_row += m_velocity.y * dt;
}


void PlayableMob::addForce(const sf::Vector2f& force) {
  m_totalForce += force;
}

void PlayableMob::applyForces() {
  // F = m*a, therefore a = F/m. Multiply by dt to get the change in velocity.
  sf::Vector2f acceleration = m_totalForce / m_mass;
  m_velocity += acceleration;
}

void PlayableMob::teleport(float column, float row) {
  m_column = column;
  m_row = row;
  // We also clear the velocity when teleporting, assuming that teleportation doesn't preserve momentum.
  m_velocity = sf::Vector2f(0, 0);
}

void PlayableMob::clearForces() {
  m_totalForce = sf::Vector2f(0, 0);
}

void PlayableMob::setVelocity(sf::Vector2f velocity) {
  m_velocity = velocity;
}

void PlayableMob::setMass(float mass) {
  m_mass = mass;
}

float PlayableMob::getMass() const {
  return m_mass;
}

sf::Vector2f PlayableMob::getVelocity() const {
  return m_velocity;
}

Rectangle PlayableMob::getBoundingBoxGivenPosition(float row, float column) const {
  float rowMin = row - 1.49f;
  float rowMax = row + 0.5f;
  float colMin = column - 0.4f;
  float colMax = column + 0.4f;

  return Rectangle(rowMin, rowMax, colMin, colMax);
}

Rectangle PlayableMob::getBoundingBox() const {
  return getBoundingBoxGivenPosition(m_row, m_column);
}

Rectangle PlayableMob::getBoundingBoxAfterUpdateX(float dt) const {
  return getBoundingBoxGivenPosition(m_row, m_column + m_velocity.x * dt);
}

Rectangle PlayableMob::getBoundingBoxAfterUpdateY(float dt) const {
  return getBoundingBoxGivenPosition(m_row + m_velocity.y * dt, m_column);
}

float PlayableMob::getRow() const {
  return m_row;
}

float PlayableMob::getColumn() const {
  return m_column;
}

sf::Vector2f PlayableMob::getCenter() const {
  return sf::Vector2f(m_column, m_row);
}

void PlayableMob::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const {
  Rectangle pos = getBoundingBox();
  sf::RectangleShape shape;
  shape.setSize(sf::Vector2f(pos.getColumnMax() - pos.getColumnMin(), pos.getRowMax() - pos.getRowMin()));
  shape.setPosition(sf::Vector2f(pos.getColumnMin(), pos.getRowMin()));
  renderTarget.draw(shape, renderStates);
}
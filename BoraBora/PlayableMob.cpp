#pragma once

#include "PlayableMob.h"
#include "KeyboardAndMouseSingleton.h"
#include "MissileMob.h"
#include "MobContainerSingleton.h"
#include <iostream>
#include "FloatingBlockMob.h"
#include "WorldBlocksSingleton.h"
#include "TextureAtlasSingleton.h"
#include "TextureType.h"
#include "BlockType.h"

PlayableMob::PlayableMob() : m_mass(1.0f), m_was(false), m_wasB(false), m_wasX(false), m_wasY(false), press(false), m_counter(static_cast<int>(BlockType::COUNT), 0), m_blockMatrix(1, 1) {
  int cnt = static_cast<int>(BlockType::COUNT) - 1;
  m_dim = 1;
  while (m_dim * m_dim < cnt) {
    m_dim++;
  }
  //m_dim = 5;
  assert(m_dim * m_dim >= cnt);
  m_blockMatrix = BlockMatrix(m_dim, m_dim);
  for (int i = 0; i < cnt; i++) {
    m_blockMatrix.setBlockType(i / m_dim, i % m_dim, static_cast<BlockType>(i));
  }
}

void PlayableMob::collect(BlockType blockType) {
  m_counter[static_cast<int>(blockType)]++;
}

bool PlayableMob::requestDelete() {
  return false;
}

void PlayableMob::killFromWorld() {

}

void PlayableMob::update(float dt) {
  for (int i = 0; i < m_blockMatrix.getRowsSize(); i++) {
    for (int j = 0; j < m_blockMatrix.getColumnsSize(); j++) {
      m_blockMatrix.setNumber(j, i, -1);
    }
  }
  for (int i = 0; i < static_cast<int>(BlockType::COUNT) - 1; i++) {
    m_blockMatrix.setNumber(i / m_dim, i % m_dim, m_counter[i]);
  }
  MobContainerSingleton::getInstance()->addFramePlayableMob(this);
  MobContainerSingleton::getInstance()->addFrameBlockMatrix(&m_blockMatrix);


  if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed(sf::Keyboard::Q)) {
    for (int i = 0; i < 100; i++) {
      std::unique_ptr<FloatingBlockMob> ptr = std::make_unique<FloatingBlockMob>();
      int column = m_column;
      int row = m_row - 3;
      ptr->teleport(column + 0.5, row + 0.5);
      ptr->setVelocity(sf::Vector2f(0, 0));
      ptr->setBlockType(BlockType::DIRT);
      MobContainerSingleton::getInstance()->addMob(std::move(ptr));
    }
  }

  if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::Space))) {
    addForce(sf::Vector2f(0, 10));
  }

  bool is = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0));
  bool isB = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1));
  bool isX = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 2));
  bool isY = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 3));
  if (!m_was && is) {
    addForce(sf::Vector2f(0, 10));
  }
  if (!m_wasB && isB) {
    std::unique_ptr<MissileMob> ptr = std::make_unique<MissileMob>();
    sf::Vector2f loc = getCenter();
    ptr->teleport(loc.x, loc.y);
    ptr->setVelocity(sf::Vector2f(+20, 0));
    if (!doesRectangleIntersectNonVoidBlocks(ptr->getBoundingBox())) {
      MobContainerSingleton::getInstance()->addMob(move(ptr));
    }
  }
  if (!m_wasX && isX) {
    std::unique_ptr<MissileMob> ptr = std::make_unique<MissileMob>();
    sf::Vector2f loc = getCenter();
    ptr->teleport(loc.x, loc.y);
    ptr->setVelocity(sf::Vector2f(-20, 0));
    if (!doesRectangleIntersectNonVoidBlocks(ptr->getBoundingBox())) {
      std::cout << "valid\n";
      MobContainerSingleton::getInstance()->addMob(move(ptr));
    }
    else {
      std::cout << "NOT\n";
    }
  }
  press ^= (!m_wasY && isY);
  m_was = is;
  m_wasB = isB;
  m_wasX = isX;
  m_wasY = isY;
  if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::L))) {
    std::unique_ptr<MissileMob> ptr = std::make_unique<MissileMob>();
    sf::Vector2f loc = getCenter();
    ptr->teleport(loc.x, loc.y);
    ptr->setVelocity(sf::Vector2f(20, 0));
    if (!doesRectangleIntersectNonVoidBlocks(ptr->getBoundingBox())) {
      MobContainerSingleton::getInstance()->addMob(move(ptr));
    }
  }
  if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::J))) {
    std::unique_ptr<MissileMob> ptr = std::make_unique<MissileMob>();
    sf::Vector2f loc = getCenter();
    ptr->teleport(loc.x, loc.y);
    ptr->setVelocity(sf::Vector2f(-20, 0));
    if (!doesRectangleIntersectNonVoidBlocks(ptr->getBoundingBox())) {
      MobContainerSingleton::getInstance()->addMob(move(ptr));
    }
  }

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
  }

  force *= 10.0f;

  force = force * dt;

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

  return Rectangle(colMin, colMax, rowMin, rowMax);
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
  sf::VertexArray varray(sf::Quads, 4);

  Rectangle pos = getBoundingBox();

  varray[0].position = sf::Vector2f(pos.getColumnMin(), pos.getRowMin());
  varray[1].position = sf::Vector2f(pos.getColumnMin(), pos.getRowMax());
  varray[2].position = sf::Vector2f(pos.getColumnMax(), pos.getRowMax());
  varray[3].position = sf::Vector2f(pos.getColumnMax(), pos.getRowMin());

  Rectangle textureRectangle = TextureAtlasSingleton::getInstance()->getTextureRectangle(TextureType::VILLAGER_SKIN);

  varray[0].texCoords = sf::Vector2f(textureRectangle.getColumnMin(), textureRectangle.getRowMin());
  varray[1].texCoords = sf::Vector2f(textureRectangle.getColumnMax(), textureRectangle.getRowMin());
  varray[2].texCoords = sf::Vector2f(textureRectangle.getColumnMax(), textureRectangle.getRowMax());
  varray[3].texCoords = sf::Vector2f(textureRectangle.getColumnMin(), textureRectangle.getRowMax());

  for (int i = 0; i < 4; i++) {
    //varray[i].color = sf::Color::Green;
  }

  //shape.setSize(sf::Vector2f(pos.getColumnMax() - pos.getColumnMin(), pos.getRowMax() - pos.getRowMin()));
  //shape.setPosition(sf::Vector2f(pos.getColumnMin(), pos.getRowMin())); 

  if (press) {
    sf::VertexArray v(sf::Lines, 2);

    Rectangle rect = getBoundingBox();

    v[0].position = sf::Vector2f((rect.getColumnMin() + rect.getColumnMax()) * 0.5f, (rect.getRowMin() + rect.getRowMax()) * 0.5f);
    v[1].position = v[0].position + sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::U), -sf::Joystick::getAxisPosition(0, sf::Joystick::V)) / 30.0f;

    for (int i = 0; i < 2; i++) {
      v[i].color = sf::Color(250, 250, 0);
    }

    int c = floor(v[1].position.x);
    int r = floor(v[1].position.y);

    {
      sf::VertexArray va(sf::Quads, 4);
      va[0].position = sf::Vector2f(c, r);
      va[1].position = sf::Vector2f(c, r + 1);
      va[2].position = sf::Vector2f(c + 1, r + 1);
      va[3].position = sf::Vector2f(c + 1, r);
      for (int i = 0; i < 4; i++) {
        va[i].color = sf::Color(0, 250, 250, 150);
      }
      renderTarget.draw(va, renderStates);
    }
    renderTarget.draw(v, renderStates);
  }



  renderStates.texture = &TextureAtlasSingleton::getInstance()->getTextureBand();
  renderTarget.draw(varray, renderStates);
}
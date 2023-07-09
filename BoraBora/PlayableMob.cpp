#include "PlayableMob.h"

PlayableMob::PlayableMob() : m_mass(1.0f) {}

void PlayableMob::update(float dt) {
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
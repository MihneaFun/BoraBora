#include "PlayableMob.h"

PlayableMob::PlayableMob() : m_mass(1.0f) {}

void PlayableMob::justElapseDtX(float dt) {
  // Move the mob according to its velocity.
  m_column += m_velocity.x * dt;
}

void PlayableMob::justElapseDtY(float dt) {
  // Move the mob according to its velocity.
  m_row += m_velocity.y * dt;
}


void PlayableMob::update(float dt) {
  applyForces();
  clearForces();
  // Move the mob according to its velocity.
  m_column += m_velocity.x * dt;
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

Rectangle PlayableMob::getBoundingBox() const {
  // You will need to define these values based on how you calculate your bounding box.
  // Here's a very simple example:
  float rowMin = m_row - 1.49f;
  float rowMax = m_row + 0.5f;
  float colMin = m_column - 0.4f;
  float colMax = m_column + 0.4f;

  return Rectangle(rowMin, rowMax, colMin, colMax);
}

Rectangle PlayableMob::getBoundingBoxAfterUpdateX(float dt) const {
  // Calculate where the mob would move after the update.
  float predictedColumn = m_column + m_velocity.x * dt;
  float predictedRow = m_row;

  // You will need to define these values based on how you calculate your bounding box.
  // Here's a very simple example, now using the predicted column and row:
  float rowMin = predictedRow - 1.49f;
  float rowMax = predictedRow + 0.5f;
  float colMin = predictedColumn - 0.4f;
  float colMax = predictedColumn + 0.4f;

  return Rectangle(rowMin, rowMax, colMin, colMax);
}

Rectangle PlayableMob::getBoundingBoxAfterUpdateY(float dt) const {
  // Calculate where the mob would move after the update.
  float predictedColumn = m_column;
  float predictedRow = m_row + m_velocity.y * dt;

  // You will need to define these values based on how you calculate your bounding box.
  // Here's a very simple example, now using the predicted column and row:
  float rowMin = predictedRow - 1.49f;
  float rowMax = predictedRow + 0.5f;
  float colMin = predictedColumn - 0.4f;
  float colMax = predictedColumn + 0.4f;

  return Rectangle(rowMin, rowMax, colMin, colMax);
}


float PlayableMob::getRow() const {
  return m_row;
}

float PlayableMob::getColumn() const {
  return m_column;
}

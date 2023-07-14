#pragma once

#include "Camera.h"
#include <iostream>

using namespace std;

void Camera::updateView() {
  sf::View view;
  view.setCenter(m_center);
  view.setSize(sf::Vector2f(m_size.x, m_size.y));
  m_texture.setView(view);
}

void Camera::create(int dimx, int dimy) {
  m_texture.create(dimx, dimy);
}

void Camera::clear() {
  m_texture.clear(sf::Color::Magenta);
}

const sf::Vector2f Camera::getCenter() {
  return m_center;
}

const sf::Vector2f Camera::getSize() {
  return m_size;
}

void Camera::setCenter(sf::Vector2f center) {
  m_center = center;
  updateView();
}

void Camera::setSize(sf::Vector2f size) {
  m_size = size;
  updateView();
}

void Camera::draw(const sf::Drawable& drawable, sf::RenderStates renderState) {
  m_texture.draw(drawable, renderState);
}

void Camera::drawCamera(Rectangle rectangle, sf::RenderTarget& renderTarget, sf::RenderStates renderState) {
  m_texture.display();

  sf::VertexArray varray(sf::Quads, 4);

  varray[0].position = sf::Vector2f(rectangle.getColumnMin(), rectangle.getColumnMin());
  varray[1].position = sf::Vector2f(rectangle.getColumnMin(), rectangle.getColumnMax());
  varray[2].position = sf::Vector2f(rectangle.getColumnMax(), rectangle.getColumnMax());
  varray[3].position = sf::Vector2f(rectangle.getColumnMax(), rectangle.getColumnMin());

  varray[0].texCoords = sf::Vector2f(0, 0);
  varray[1].texCoords = sf::Vector2f(0, m_texture.getTexture().getSize().y);
  varray[2].texCoords = sf::Vector2f(m_texture.getTexture().getSize().x, m_texture.getTexture().getSize().y);
  varray[3].texCoords = sf::Vector2f(m_texture.getTexture().getSize().x, 0);

  renderState.texture = &m_texture.getTexture();

  renderTarget.draw(varray, renderState);
}

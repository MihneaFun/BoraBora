#pragma once

#include "KeyboardAndMouseSingleton.h"
#include <SFML/Window/Window.hpp>

KeyboardAndMouseSingleton* KeyboardAndMouseSingleton::m_instance = nullptr;

KeyboardAndMouseSingleton::KeyboardAndMouseSingleton() {
  m_keyStates.fill(false);
  m_lastKeyStates.fill(false);

  m_buttonStates.fill(false);
  m_lastButtonStates.fill(false);
}

KeyboardAndMouseSingleton* KeyboardAndMouseSingleton::getInstance() {
  return m_instance;
}

void KeyboardAndMouseSingleton::createInstance() {
  if (!m_instance) {
    m_instance = new KeyboardAndMouseSingleton();
  }
}

void KeyboardAndMouseSingleton::destroyInstance() {
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

bool KeyboardAndMouseSingleton::isKeyPressed(sf::Keyboard::Key key) {
  return m_keyStates[key];
}

bool KeyboardAndMouseSingleton::isButtonPressed(sf::Mouse::Button button) {
  return m_buttonStates[button];
}

bool KeyboardAndMouseSingleton::isKeyJustPressed(sf::Keyboard::Key key) {
  return m_keyStates[key] && !m_lastKeyStates[key];
}

bool KeyboardAndMouseSingleton::isKeyJustReleased(sf::Keyboard::Key key) {
  return !m_keyStates[key] && m_lastKeyStates[key];
}

bool KeyboardAndMouseSingleton::isButtonJustPressed(sf::Mouse::Button button) {
  return m_buttonStates[button] && !m_lastButtonStates[button];
}

bool KeyboardAndMouseSingleton::isButtonJustReleased(sf::Mouse::Button button) {
  return !m_buttonStates[button] && m_lastButtonStates[button];
}

void KeyboardAndMouseSingleton::update() {
  m_lastKeyStates = m_keyStates;
  m_lastButtonStates = m_buttonStates;

  for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
    m_keyStates[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
  }

  for (int i = 0; i < sf::Mouse::ButtonCount; ++i) {
    m_buttonStates[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
  }
}

#pragma once


#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>

class KeyboardAndMouseSingleton {
private:
  KeyboardAndMouseSingleton();
  KeyboardAndMouseSingleton(const KeyboardAndMouseSingleton&) = delete;
  KeyboardAndMouseSingleton(KeyboardAndMouseSingleton&&) = delete;

  std::array<bool, sf::Keyboard::KeyCount> m_keyStates;
  std::array<bool, sf::Keyboard::KeyCount> m_lastKeyStates;

  std::array<bool, sf::Mouse::ButtonCount> m_buttonStates;
  std::array<bool, sf::Mouse::ButtonCount> m_lastButtonStates;

  static KeyboardAndMouseSingleton* m_instance;

public:
  static KeyboardAndMouseSingleton* getInstance();
  static void destroyInstance();
  static void createInstance();

  bool isKeyPressed(sf::Keyboard::Key key);
  bool isButtonPressed(sf::Mouse::Button button);

  bool isKeyJustPressed(sf::Keyboard::Key key);
  bool isKeyJustReleased(sf::Keyboard::Key key);

  bool isButtonJustPressed(sf::Mouse::Button button);
  bool isButtonJustReleased(sf::Mouse::Button button);

  void update();
};


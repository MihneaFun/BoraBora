#include <SFML/Graphics.hpp>

#include "TextureType.h"
#include "BlockType.h"

#include "KeyboardAndMouseSingleton.h"
#include "MissileMob.h"
#include "PlayableMob.h"
#include "TextureAtlasSingleton.h"
#include "GeneralTextureLinkSingleton.h"
#include "GeneralTextureManagerSingleton.h"
#include "WorldBlocksSingleton.h"
#include "WorldDrawerSingleton.h"

#include "Rectangle.h"
#include "Camera.h"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(900, 900), "SFML works!");

  // Texture stuff:

  KeyboardAndMouseSingleton::createInstance();
  GeneralTextureLinkSingleton::createInstance();
  GeneralTextureManagerSingleton::createInstance();
  TextureAtlasSingleton::createInstance();
  WorldDrawerSingleton::createInstance(window);

  // World stuff:
  WorldBlocksSingleton::createInstance(10000, 200);

  vector<unique_ptr<GenericMob>> mobs;

  mobs.push_back(make_unique<PlayableMob>());

  mobs[0]->teleport(10, 20);

  sf::View view;
  view.setSize(sf::Vector2f(1, -1));
  view.setCenter(sf::Vector2f(0.5, 0.5));
  window.setView(view);

  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  int frames = 0;


  sf::Clock frameClock;
  float mob_speed = 10;

  float x = 0, y = 0;

  Rectangle bigWindowRectangle(0, 1, 0, 1);

  const float fixedDT = 0.005;
  float currentDT = 0;
  bool was = 0, wasB = 0, wasX = 0;
  while (window.isOpen()) {
    currentDT += frameClock.restart().asSeconds();
    if (currentDT < fixedDT) {
      sf::sleep(sf::seconds(fixedDT - currentDT));
      continue;
    }
    currentDT -= fixedDT;
    KeyboardAndMouseSingleton::getInstance()->update();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::Space))) {
      mobs[0]->addForce(sf::Vector2f(0, 10));
    }

    bool is = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0));
    bool isB = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1));
    bool isX = (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 2));
    if (!was && is) {
      mobs[0]->addForce(sf::Vector2f(0, 10));
    }
    if (!wasB && isB) {
      mobs.push_back(make_unique<MissileMob>());
      sf::Vector2f loc = mobs[0]->getCenter();
      mobs.back()->teleport(loc.x, loc.y);
      mobs.back()->setVelocity(sf::Vector2f(20, 0));
      if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
        mobs.back().reset();
        mobs.pop_back();
        cout << "it hits\n";
      }
    }
    if (!wasX && isX) {
      mobs.push_back(make_unique<MissileMob>());
      sf::Vector2f loc = mobs[0]->getCenter();
      mobs.back()->teleport(loc.x, loc.y);
      mobs.back()->setVelocity(sf::Vector2f(-20, 0));
      if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
        mobs.back().reset();
        mobs.pop_back();
        cout << "it hits\n";
      }
    }
    was = is;
    wasB = isB;
    wasX = isX;
    if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::L))) {
      mobs.push_back(make_unique<MissileMob>());
      sf::Vector2f loc = mobs[0]->getCenter();
      mobs.back()->teleport(loc.x, loc.y);
      mobs.back()->setVelocity(sf::Vector2f(20, 0));
      if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
        mobs.back().reset();
        mobs.pop_back();
        cout << "it hits\n";
      }
    }
    if (KeyboardAndMouseSingleton::getInstance()->isKeyJustPressed((sf::Keyboard::J))) {
      mobs.push_back(make_unique<MissileMob>());
      sf::Vector2f loc = mobs[0]->getCenter();
      mobs.back()->teleport(loc.x, loc.y);
      mobs.back()->setVelocity(sf::Vector2f(-20, 0));
      if (doesRectangleIntersectNonVoidBlocks(mobs.back()->getBoundingBox())) {
        mobs.back().reset();
        mobs.pop_back();
        cout << "it hits\n";
      }
    }

    for (auto& mob : mobs) {
      mob->addForce(sf::Vector2f(-mob->getVelocity().x, 0) * 0.004f);
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

    force *= mob_speed;

    // Scale the force by dt to make the movement time-dependent
    force = force * fixedDT;

    // Apply the calculated force to the mob
    mobs[0]->addForce(force);

    for (auto& mob : mobs) {
      mob->addForce(sf::Vector2f(0, -0.1));
    }

    timeSinceLastUpdate += clock.restart();
    frames++;

    if (timeSinceLastUpdate > sf::seconds(1.0f)) {
      std::cout << "FPS: " << frames << ", " << (int)mobs.size() << "\n";
      frames = 0;
      timeSinceLastUpdate -= sf::seconds(1.0f);
    }

    window.clear();

    Rectangle windowRectangle(0.20, 0.8, 0.20, 0.8);

    if (PlayableMob* mob = dynamic_cast<PlayableMob*>(mobs[0].get())) {
      x = mob->getColumn() - 15;
      y = mob->getRow() - 15;
    }
    else {
      cout << "Why in the name of Newton's apple isn't the first mob a PlayableMob?\n";
      exit(0);;
    }

    Rectangle worldRectangle(y, y + 30, x + 0, x + 30);

    if (0) {
      sf::RectangleShape shape;
      shape.setSize(sf::Vector2f(1000, 1000));
      shape.setPosition(sf::Vector2f(0, 0));
      shape.setFillColor(sf::Color::Yellow);
      window.draw(shape);
    }

    WorldDrawerSingleton::getInstance(window)->drawWorldOnWindow(windowRectangle, worldRectangle, mobs);

    if (0) {
      sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
      sf::CircleShape shape;

      shape.setRadius(0.1);
      shape.setOrigin(sf::Vector2f(0.1, 0.1));
      shape.setPosition(worldPos);
      shape.setFillColor(sf::Color::Green);

      window.draw(shape);
    }

    for (auto& mob : mobs) {
      mob->update(fixedDT);
    }

    window.display();

    for (int i = 0; i < (int)mobs.size(); i++) {
      if (mobs[i]->requestDelete()) {
        Rectangle bb = mobs[i]->getBoundingBox();
        for (int row = bb.getRowMin() - 2; row <= bb.getRowMax() + 2; row++) {
          for (int column = bb.getColumnMin() - 2; column <= bb.getColumnMin() + 2; column++) {
            if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
              continue;
            }
            WorldBlocksSingleton::getInstance()->setBlockType(column, row, BlockType::VOID);
          }
        }
        swap(mobs[i], mobs.back());
        mobs.back().reset();
        mobs.pop_back();
        i--;
      }
    }
  }

  return 0;
}
// dynamic cast e foarte foarte foarte shitty pe hot path 
// my code stinks badly 
// to Do : delete update from Generic Mob class and all mob classes
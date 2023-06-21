#include <SFML/Graphics.hpp>

#include "TextureType.h"
#include "BlockType.h"

#include "PlayableMob.h"
#include "TextureAtlasSingleton.h"
#include "GeneralTextureLinkSingleton.h"
#include "GeneralTextureManagerSingleton.h"
#include "WorldBlocksSingleton.h"
#include "WorldDrawerSingleton.h"

#include "Rectangle.h"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(900, 900), "SFML works!");

  // Texture stuff:

  GeneralTextureLinkSingleton::createInstance();
  GeneralTextureManagerSingleton::createInstance();
  TextureAtlasSingleton::createInstance();
  WorldDrawerSingleton::createInstance(window);
  // World stuff:
  WorldBlocksSingleton::createInstance(10000, 200);

  PlayableMob mob;
  mob.teleport(10, 20);

  sf::View view;
  view.setSize(sf::Vector2f(1, -1)); // Note the negative y value here
  view.setCenter(sf::Vector2f(0.5, 0.5));
  window.setView(view);

  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  int frames = 0;


  sf::Clock frameClock;
  float mob_speed = 10;

  float x = 0, y = 0;
  bool wasWPressed = false, wasAPressed = false, wasSPressed = false, wasDPressed = false;
  bool wasSpacePressed = false;

  Rectangle bigWindowRectangle(0, 1, 0, 1);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    // Check for just pressed
    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && abs(mob.getVelocity().y) <= 0.001) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
       //mob.addForce(sf::Vector2f(0, 0.5));
      if (!wasSpacePressed) {
        mob.addForce(sf::Vector2f(0, 10));
        //cout << "aici\n";
        wasSpacePressed = true;
      }
    }
    else {
      wasSpacePressed = false;
    }
    

    // Check for just pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      if (!wasWPressed) {
        y += 1;
        wasWPressed = true;
      }
    }
    else {
      wasWPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      if (!wasSPressed) {
        y -= 1;
        wasSPressed = true;
      }
    }
    else {
      wasSPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      if (!wasAPressed) {
        x -= 1;
        wasAPressed = true;
      }
    }
    else {
      wasAPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      if (!wasDPressed) {
        x += 1;
        wasDPressed = true;
      }
    }
    else {
      wasDPressed = false;
    }

    float dt = frameClock.restart().asSeconds();
    mob.addForce(sf::Vector2f(-mob.getVelocity().x, 0) * 0.004f);

    // Handle mob movement
    sf::Vector2f force(0.0f, 0.0f);

    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    //  force.y -= 1;
    //}
    //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    //  force.y += 1;
    //}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      force.x -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      force.x += 1;
    }

    // Normalize force to ensure consistent speed regardless of direction
    if (force.x != 0.0f || force.y != 0.0f) {
      force = force / std::sqrt(force.x * force.x + force.y * force.y);
    }

    force *= mob_speed;

    // Scale the force by dt to make the movement time-dependent
    force = force * dt;

    // Apply the calculated force to the mob
    mob.addForce(force);

    mob.addForce(sf::Vector2f(0, -0.04));


    timeSinceLastUpdate += clock.restart();
    frames++;

    if (timeSinceLastUpdate > sf::seconds(1.0f)) {
      std::cout << "FPS: " << frames << "\n";
      frames = 0;
      timeSinceLastUpdate -= sf::seconds(1.0f);
    }

    window.clear();

    //Rectangle windowRectangle(0.05, 0.95, 0.05, 0.95);
    //Rectangle windowRectangle(0.2, 0.8, 0.2, 0.8);
    Rectangle windowRectangle(0.20, 0.8, 0.20, 0.8);
    //Rectangle worldRectangle(y, y + 49, x + 0, x + 49);

    x = mob.getColumn() - 15;
    y = mob.getRow() - 15;

    Rectangle worldRectangle(y, y + 30, x + 0, x + 30);

    if (0) {
      sf::RectangleShape shape;
      shape.setSize(sf::Vector2f(0.8, 0.8));
      shape.setPosition(sf::Vector2f(0.1, 0.1));
      shape.setFillColor(sf::Color::Yellow);
      window.draw(shape);
    }


    WorldDrawerSingleton::getInstance(window)->drawWorldOnWindow(windowRectangle, worldRectangle);

    if (0) {
      sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
      sf::CircleShape shape;

      shape.setRadius(0.1);
      shape.setOrigin(sf::Vector2f(0.1, 0.1));
      shape.setPosition(worldPos);
      shape.setFillColor(sf::Color::Green);

      window.draw(shape);
    }

    if (1) {
      Rectangle pos = getNewCoords(worldRectangle, windowRectangle, mob.getBoundingBox());
      sf::RectangleShape shape;
      shape.setSize(sf::Vector2f(pos.getColumnMax() - pos.getColumnMin(), pos.getRowMax() - pos.getRowMin()));
      shape.setPosition(sf::Vector2f(pos.getColumnMin(), pos.getRowMin()));
      window.draw(shape);
    }

    if (1) {
      mob.applyForces();
      mob.clearForces();
      // Update the mob
      {
        bool invalid = false;
        float curdt = dt;
        for (int iter = 1; iter <= 30, curdt *= 0.5f; iter++) {
          //cout << " --> " << curdt << "\n";
          Rectangle rect = mob.getBoundingBoxAfterUpdateX(curdt);
          int startRow = rect.getRowMin() - 2, endRow = rect.getRowMax() + 2;
          int startColumn = rect.getColumnMin() - 2, endColumn = rect.getColumnMax() + 2;
          bool valid = true;
          for (int row = startRow; row <= endRow && valid; ++row) {
            for (int column = startColumn; column <= endColumn && valid; ++column) {

              if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
                continue;
              }

              if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
                continue;
              }

              BlockType blockType = WorldBlocksSingleton::getInstance()->getBlockType(column, row);

              Rectangle thisRectangle(row, row + 1, column, column + 1);

              if (doRectanglesIntersect(thisRectangle, rect)) {
                if (iter == 10) {
                  Rectangle lol = getNewCoords(worldRectangle, windowRectangle, thisRectangle);
                  lol = thisRectangle;
                  //lol = getNewCoords(windowRectangle, bigWindowRectangle, lol);
                  sf::RectangleShape shape;
                  shape.setSize(sf::Vector2f(lol.getColumnMax() - lol.getColumnMin(), lol.getRowMax() - lol.getRowMin()));
                  shape.setPosition(sf::Vector2f(lol.getColumnMin(), lol.getRowMin()));
                  window.draw(shape);
                }
                valid = false;
              }
            }
          }
          if (iter == 20) {
            //cout << " : " << fixed << setprecision(9) << curdt << "\n";
          }
          if (valid) {
            mob.justElapseDtX(curdt);
          }
          else {
            invalid = true;
          }
        }
        if (invalid) {
          mob.setVelocity(sf::Vector2f(0, mob.getVelocity().y));
        }
      }
      {
        bool invalid = false;
        float curdt = dt;
        for (int iter = 1; iter <= 30, curdt *= 0.5f; iter++) {
          //cout << " --> " << curdt << "\n";
          Rectangle rect = mob.getBoundingBoxAfterUpdateY(curdt);
          int startRow = rect.getRowMin() - 2, endRow = rect.getRowMax() + 2;
          int startColumn = rect.getColumnMin() - 2, endColumn = rect.getColumnMax() + 2;
          bool valid = true;
          for (int row = startRow; row <= endRow && valid; ++row) {
            for (int column = startColumn; column <= endColumn && valid; ++column) {

              if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
                continue;
              }

              if (WorldBlocksSingleton::getInstance()->getBlockType(column, row) == BlockType::VOID) {
                continue;
              }

              BlockType blockType = WorldBlocksSingleton::getInstance()->getBlockType(column, row);

              Rectangle thisRectangle(row, row + 1, column, column + 1);

              if (doRectanglesIntersect(thisRectangle, rect)) {
                if (iter == 10) {
                  Rectangle lol = getNewCoords(worldRectangle, windowRectangle, thisRectangle);
                  lol = thisRectangle;
                  //lol = getNewCoords(windowRectangle, bigWindowRectangle, lol);
                  sf::RectangleShape shape;
                  shape.setSize(sf::Vector2f(lol.getColumnMax() - lol.getColumnMin(), lol.getRowMax() - lol.getRowMin()));
                  shape.setPosition(sf::Vector2f(lol.getColumnMin(), lol.getRowMin()));
                  window.draw(shape);
                }
                valid = false;
              }
            }
          }
          if (valid) {
            mob.justElapseDtY(curdt);
          }
          else {
            invalid = true;
          }
        }
        if (invalid) {
          mob.setVelocity(sf::Vector2f(mob.getVelocity().x, 0));
        }
      }
    }

    window.display();

  }

  return 0;
}
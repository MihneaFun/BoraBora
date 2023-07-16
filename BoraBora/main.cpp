#include <SFML/Graphics.hpp>

#include "TextureType.h"
#include "BlockType.h"

#include "MobContainerSingleton.h"
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
  MobContainerSingleton::createInstance();

  {
    unique_ptr<GenericMob> ptr = make_unique<PlayableMob>();
    ptr->teleport(10, 20);
    MobContainerSingleton::getInstance()->addMob(move(ptr));
  }


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

    timeSinceLastUpdate += clock.restart();
    frames++;

    if (timeSinceLastUpdate > sf::seconds(1.0f)) {
      std::cout << "FPS: " << frames << ", " << MobContainerSingleton::getInstance()->getMobCount() << "\n";
      frames = 0;
      timeSinceLastUpdate -= sf::seconds(1.0f);
    }
    
    window.clear();
    Rectangle windowRectangle(0.2, 0.8, 0.2, 0.8);

    {
      float L = 0.2, R = 0.8;
      int numPrint = static_cast<int>(BlockType::COUNT) - 1;
      float dim = (R - L) / numPrint;
      sf::VertexArray vertexArray(sf::Quads, 4 * numPrint);
      for (int i = 0; i < 4 * numPrint; i++) {
        vertexArray[i] = sf::Vertex();
      }
      int yp = 0;
      for (int i = 0; i < numPrint; i++) {
        BlockType blockType = static_cast<BlockType>(i);

        sf::Vertex topLeft = sf::Vector2f(0.05, L + dim * i);
        sf::Vertex topRight = sf::Vector2f(0.15, L + dim * i);
        sf::Vertex bottomRight = sf::Vector2f(0.15, L + dim * (i + 1));
        sf::Vertex bottomLeft = sf::Vector2f(0.05, L + dim * (i + 1));

        


        Rectangle textureRectangle = TextureAtlasSingleton::getInstance()->getTextureRectangle(blockType);
        topLeft.texCoords = { textureRectangle.getColumnMin(), textureRectangle.getRowMin() };
        topRight.texCoords = { textureRectangle.getColumnMax(), textureRectangle.getRowMin() };
        bottomRight.texCoords = { textureRectangle.getColumnMax(), textureRectangle.getRowMax() };
        bottomLeft.texCoords = { textureRectangle.getColumnMin(), textureRectangle.getRowMax() };

        vertexArray[yp] = topLeft;
        vertexArray[yp + 1] = topRight;
        vertexArray[yp + 2] = bottomRight;
        vertexArray[yp + 3] = bottomLeft;

        yp += 4;

      }
      //cout << " : " << window.getView().getCenter().x << " " << window.getView().getCenter().y << "\n";
      window.draw(vertexArray, &TextureAtlasSingleton::getInstance()->getTextureBand());
    }

    {
      Rectangle r = MobContainerSingleton::getInstance()->getMob(0)->getBoundingBox();
      float xm, ym;
      xm = (r.getColumnMin() + r.getColumnMax()) * 0.5;
      ym = (r.getRowMin() + r.getRowMax()) * 0.5;
      x = xm - 15;
      y = ym - 15;
    }
    Rectangle worldRectangle(y, y + 30, x + 0, x + 30);

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

    MobContainerSingleton::getInstance()->update(fixedDT);



    window.display();
  }

  return 0;
}
// de adaugat set Smooth la texturi sau Mipmap <-- articole pe wikipedia
// ctrl shift f => ctrl find peste tot
// rider -> jetBrains
// implement dda
// ctrl shift F shady
// 
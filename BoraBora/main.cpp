#include <SFML/Graphics.hpp>

#include "TextSingleton.h"

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
#include "BlockMatrix.h"

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
  TextSingleton::createInstance();

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

  Rectangle windowRectangle(0.2, 0.8, 0.2, 0.8);

  int tog = 0;

  bool was5 = 0;

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
    if (timeSinceLastUpdate > sf::seconds(1.0f)) {
      std::cout << "FPS: " << frames << ", " << MobContainerSingleton::getInstance()->getMobCount() << " " << x << " " << y << "\n";
      frames = 0;
      timeSinceLastUpdate -= sf::seconds(1.0f);
    }
    timeSinceLastUpdate += clock.restart();
    frames++;

    
    
    bool is5 = sf::Joystick::isButtonPressed(0, 5);

    tog ^= (is5 && !was5);
    was5 = is5;

    window.clear();
    //Rectangle windowRectangle(0.2, 0.5, 0.2, 0.8);

    float speed = 0.5;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      windowRectangle = Rectangle(windowRectangle.getColumnMin() + fixedDT * speed, windowRectangle.getColumnMax() + fixedDT * speed, windowRectangle.getRowMin(), windowRectangle.getRowMax());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
      windowRectangle = Rectangle(windowRectangle.getColumnMin() - fixedDT * speed, windowRectangle.getColumnMax() - fixedDT * speed, windowRectangle.getRowMin(), windowRectangle.getRowMax());
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
      windowRectangle = Rectangle(windowRectangle.getColumnMin(), windowRectangle.getColumnMax(), windowRectangle.getRowMin() + fixedDT * speed, windowRectangle.getRowMax() + fixedDT * speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
      windowRectangle = Rectangle(windowRectangle.getColumnMin(), windowRectangle.getColumnMax(), windowRectangle.getRowMin() - fixedDT * speed, windowRectangle.getRowMax() - fixedDT * speed);
    }

    
    float speed2 = 0.2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
      float inc = windowRectangle.getColumnMax() - windowRectangle.getColumnMin();
      inc *= speed2 * fixedDT;
      windowRectangle = Rectangle(windowRectangle.getColumnMin() + inc, windowRectangle.getColumnMax() - inc, windowRectangle.getRowMin(), windowRectangle.getRowMax());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
      float inc = windowRectangle.getColumnMax() - windowRectangle.getColumnMin();
      inc *= speed2 * fixedDT;
      windowRectangle = Rectangle(windowRectangle.getColumnMin() - inc, windowRectangle.getColumnMax() + inc, windowRectangle.getRowMin(), windowRectangle.getRowMax());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)) {
      float inc = windowRectangle.getRowMax() - windowRectangle.getRowMin();
      inc *= speed2 * fixedDT;
      windowRectangle = Rectangle(windowRectangle.getColumnMin(), windowRectangle.getColumnMax() - inc, windowRectangle.getRowMin() + inc, windowRectangle.getRowMax() - inc);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
      float inc = windowRectangle.getRowMax() - windowRectangle.getRowMin();
      inc *= speed2 * fixedDT;
      windowRectangle = Rectangle(windowRectangle.getColumnMin(), windowRectangle.getColumnMax(), windowRectangle.getRowMin() - inc, windowRectangle.getRowMax() + inc);
    }

    if (0) {
      float L = 0.2, R = 0.8;
      int numPrint = static_cast<int>(BlockType::COUNT) - 1;
      float m_dim = (R - L) / numPrint;
      sf::VertexArray vertexArray(sf::Quads, 4 * numPrint);
      for (int i = 0; i < 4 * numPrint; i++) {
        vertexArray[i] = sf::Vertex();
      }
      int yp = 0;
      for (int i = 0; i < numPrint; i++) {
        BlockType blockType = static_cast<BlockType>(i);

        sf::Vertex topLeft = sf::Vector2f(0.05, L + m_dim * i);
        sf::Vertex topRight = sf::Vector2f(0.15, L + m_dim * i);
        sf::Vertex bottomRight = sf::Vector2f(0.15, L + m_dim * (i + 1));
        sf::Vertex bottomLeft = sf::Vector2f(0.05, L + m_dim * (i + 1));

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

    Rectangle worldRectangle(x, x + 30, y + 0, y + 30);

    if (!tog) {
      WorldDrawerSingleton::getInstance(window)->drawWorldOnWindow(windowRectangle, worldRectangle);
      MobContainerSingleton::getInstance()->update(fixedDT);
    }
    
    BlockMatrix* m = nullptr;
    if ((int)MobContainerSingleton::getInstance()->m_frame_block_matrices.size() == 1) m = MobContainerSingleton::getInstance()->m_frame_block_matrices[0];
    //cout << " ooo " << (int)MobContainerSingleton::getInstance()->m_frame_block_matrices.size() << "\n";
    //assert((int)MobContainerSingleton::getInstance()->m_frame_block_matrices.size() == 1);
    //BlockMatrix* m = MobContainerSingleton::getInstance()->m_frame_block_matrices[0];

    if (0) {
      sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
      sf::CircleShape shape;

      shape.setRadius(0.1);
      shape.setOrigin(sf::Vector2f(0.1, 0.1));
      shape.setPosition(worldPos);
      shape.setFillColor(sf::Color::Green);

      window.draw(shape);
    }

    if (m) {
      m->setRectangle(windowRectangle);
      m->update(fixedDT);
    }

    if (tog) {
      assert(m);
      window.draw(*m);
    }
    
    sf::Text text;

    window.display();
  }

  return 0;
}
// de adaugat set Smooth la texturi sau Mipmap <-- articole pe wikipedia
// ctrl shift f => ctrl find peste tot
// rider -> jetBrains
// implement dda
// ctrl shift F shady
// check out
// la mobi problema e draw : addQuads ca la particle system in loc de draw
// debug -> performance profiler
// delete get bounding box after ...
// epsilon sa il fac si mai mic in genericMob
// duck game -> steam
// quake 3 arena (3d) mai vechi
// class ControlPlayableMob to do InputProcessingComponent -> queue de comenzi
// abstractizez conceptul de InputDevice
// GameInputManager System
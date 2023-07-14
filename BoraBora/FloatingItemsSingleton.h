#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include "BlockType.h"
#include "PlayableMob.h"

class FloatingItemsSingleton : public sf::Drawable {
private:
  static FloatingItemsSingleton* m_instance;
  int m_maxSize;
  std::vector<std::pair<BlockType, std::pair<int, int>>> m_floatingItems;
  sf::VertexArray m_varray;
  FloatingItemsSingleton();
  FloatingItemsSingleton(const FloatingItemsSingleton& other) = delete;
  FloatingItemsSingleton(FloatingItemsSingleton&& other) = delete;

public:

  void loadVertexArray();
  static FloatingItemsSingleton* getInstance();
  static void destroy();
  static void createInstance();

  void set_max_size(int max_size);
  void add(BlockType blockType, int column, int row);
  void collect(PlayableMob& mob);
  void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderstates) const override;
};


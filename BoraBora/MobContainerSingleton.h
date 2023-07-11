#pragma once

#include "GenericMob.h"
#include <memory>
#include <vector>
#include <cassert>

class MobContainerSingleton {
private:
  MobContainerSingleton() = default;
  MobContainerSingleton(const MobContainerSingleton& other) = delete;
  MobContainerSingleton(MobContainerSingleton&& other) = delete;
  static MobContainerSingleton* m_instance;

  std::vector<std::unique_ptr<GenericMob>> m_mobs;
public:
  
  const std::unique_ptr<GenericMob>& getMob(int index) const {
    assert(0 <= index && index < (int)m_mobs.size());
    return m_mobs[index];
  }

  int getMobCount() const {
    return (int)m_mobs.size();
  }

  static MobContainerSingleton* getInstance();
  static void createInstance();
  static void destroyInstance();

  void addMob(std::unique_ptr<GenericMob> mob);
  void eraseLastMob();
  void update(float dt);
};

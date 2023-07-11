#pragma once

#include "GenericMob.h"
#include <memory>
#include <vector>

class MobContainerSingleton {
private:
  MobContainerSingleton() = default;
  MobContainerSingleton(const MobContainerSingleton& other) = delete;
  MobContainerSingleton(MobContainerSingleton&& other) = delete;
  static MobContainerSingleton* m_instance;

public:
  std::vector<std::unique_ptr<GenericMob>> m_mobs;

  static MobContainerSingleton* getInstance();
  static void createInstance();
  static void destroyInstance();

  void addMob(std::unique_ptr<GenericMob> mob);
  void eraseLastMob();
  void updateAllMobs(float dt);
};

#pragma once

#include "MobContainerSingleton.h"
#include "GenericMob.h"

MobContainerSingleton* MobContainerSingleton::m_instance;

MobContainerSingleton* MobContainerSingleton::getInstance() {
  if (m_instance == nullptr) {
    createInstance();
  }
  return m_instance;
}

void MobContainerSingleton::createInstance() {
  if (m_instance == nullptr) {
    m_instance = new MobContainerSingleton();
  }
}

void MobContainerSingleton::destroyInstance() {
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

void MobContainerSingleton::addMob(std::unique_ptr<GenericMob> mob) {
  m_mobs.push_back(std::move(mob));
}

void MobContainerSingleton::eraseLastMob() {
  if (!m_mobs.empty()) {
    m_mobs.pop_back();
  }
}

void MobContainerSingleton::updateAllMobs(float dt) {
  for (int i = 0; i < (int)m_mobs.size(); i++) {
    m_mobs[i]->update(dt, m_mobs);
  }
}

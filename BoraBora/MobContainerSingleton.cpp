#pragma once

#include "MobContainerSingleton.h"
#include "GenericMob.h"
#include "WorldBlocksSingleton.h"

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

void MobContainerSingleton::update(float dt) {
  for (int i = 0; i < (int)m_mobs.size(); i++) {
    m_mobs[i]->update(dt);
  }
  for (int i = 0; i < (int)m_mobs.size(); i++) {
    if (m_mobs[i]->requestDelete()) {
      Rectangle bb = m_mobs[i]->getBoundingBox();
      for (int row = bb.getRowMin() - 2; row <= bb.getRowMax() + 2; row++) {
        for (int column = bb.getColumnMin() - 2; column <= bb.getColumnMin() + 2; column++) {
          if (column < 0 || column >= WorldBlocksSingleton::getInstance()->getWidth() || row < 0 || row >= WorldBlocksSingleton::getInstance()->getHeight()) {
            continue;
          }
          WorldBlocksSingleton::getInstance()->setBlockType(column, row, BlockType::VOID);
        }
      }
      swap(m_mobs[i], m_mobs.back());
      m_mobs.back().reset();
      m_mobs.pop_back();
      i--;
    }
  }
}

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


void MobContainerSingleton::addFramePlayableMob(PlayableMob* mob) {
  m_frame_playable_mobs.push_back(mob);
}

void MobContainerSingleton::addFrameFloatingBlockMob(FloatingBlockMob* mob) {
  m_frame_floating_block_mobs.push_back(mob);
}


void MobContainerSingleton::eraseLastMob() {
  if (!m_mobs.empty()) {
    m_mobs.pop_back();
  }
}

#include <iostream>

void MobContainerSingleton::update(float dt) {
  m_frame_playable_mobs.clear();
  m_frame_floating_block_mobs.clear();
  for (int i = 0; i < (int)m_mobs.size(); i++) {
    m_mobs[i]->update(dt);
  }
  for (int i = 0; i < (int)m_frame_floating_block_mobs.size(); i++) {
    for (int j = 0; j < (int)m_frame_playable_mobs.size(); j++) {
      if (doRectanglesIntersect(m_frame_floating_block_mobs[i]->getBoundingBox(), m_frame_playable_mobs[j]->getBoundingBox())) {
        //std::cout << "aici\n";
        m_frame_floating_block_mobs[i]->setCollected();
        break;
      }
    }
  }
  //std::cout << " : " << m_frame_playable_mobs.size() << " and " << m_frame_floating_block_mobs.size() << "\n";
  for (int i = 0; i < (int)m_mobs.size(); i++) {
    if (m_mobs[i]->requestDelete()) {
      swap(m_mobs[i], m_mobs.back());
      m_mobs.back()->killFromWorld();
      m_mobs.back().reset();
      m_mobs.pop_back();
      i--;
    }
  }
}

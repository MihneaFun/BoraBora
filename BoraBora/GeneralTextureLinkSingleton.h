#pragma once

#include "TextureType.h"
#include <string>
#include <vector>
#include <cassert>

class GeneralTextureLinkSingleton {
private:
  static GeneralTextureLinkSingleton* m_instance;
  std::vector<std::string> m_textureLinks;

  GeneralTextureLinkSingleton() = default;
  GeneralTextureLinkSingleton(const GeneralTextureLinkSingleton& other) = delete;
  GeneralTextureLinkSingleton(GeneralTextureLinkSingleton&& other) = delete;

public:
  static GeneralTextureLinkSingleton* getInstance();
  static void destroy();
  static void createInstance();

  std::string getLink(TextureType type);
  void setLink(TextureType type, const std::string& link);

  static void loadAllLinks();
};

#ifndef __INCLUDE_RESOURCES_MANAGER_H__
#define __INCLUDE_RESOURCES_MANAGER_H__

#include "imgui.h"
#include <map>
#include <string>
#include <memory>

class Texture {
public:
  Texture(ImTextureID ID, float w, float h) : ID(ID), size(w, h) {}
  Texture(ImTextureID ID, int w, int h) : ID(ID), size((float)w, (float)h) {}

  ImTextureID ID;
  ImVec2 size;

  ~Texture(){};
};

class ResManager {
  using StorageTy = std::map<std::string, std::unique_ptr<Texture>>;

  StorageTy storage;
  std::string base_dir;

public:
  ResManager(std::string Directory = "res/") : base_dir(Directory) {}
  Texture *tryLoad(std::string ResourceName, std::string ResourceID);
  Texture *get(std::string ResourceName);

  static ResManager *getInstance();
  virtual ~ResManager(){};
};

#endif // __INCLUDE_RESOURCES_MANAGER_H__
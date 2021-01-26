#include "ResourcesManager.h"

ResManager Instance;

ResManager *ResManager::getInstance() { return &Instance; }

bool ResManager::tryLoad(std::string ResourceName, std::string ResourceID) {
  auto& result = storage[ResourceName];
  if (result)
    return false; //Already exists

  //TODO: try loading the resource
  if (ResourceID.empty()) {
    ImGuiIO &io = ImGui::GetIO();
    result.reset(new Texture(io.Fonts->TexID, io.Fonts->TexWidth, io.Fonts->TexHeight));
    return true;
  }

  return false;
}

Texture *ResManager::get(std::string ResourceName) {
  auto result = storage.find(ResourceName);
  return result == storage.end() ? nullptr : result->second.get();
}
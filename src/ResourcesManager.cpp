#include "ResourcesManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <d3d9.h>

ResManager Instance;

ResManager *ResManager::getInstance() { return &Instance; }

LPDIRECT3DDEVICE9 ResManager::device;

bool LoadTextureFromFile(std::string filename, PDIRECT3DTEXTURE9 *out_texture, int *out_width, int *out_height)
{
  int width = 0;
  int height = 0;

  unsigned char* data = stbi_load(filename.c_str(), &width, &height, nullptr, 4); 
  if (!data)
    return false;

  bool result = false;
  PDIRECT3DTEXTURE9 texture;
  HRESULT hr = ResManager::device->CreateTexture(width, height, 1,
                                                 D3DUSAGE_DYNAMIC, D3DFMT_A8B8G8R8,
                                                 D3DPOOL_DEFAULT, &texture, nullptr);

  if (hr != D3D_OK)
    goto cleanup;
  
  D3DLOCKED_RECT locked_rect;
  if (texture->LockRect(0, &locked_rect, NULL, 0) != D3D_OK)
    goto cleanup;

  int bytes_per_pixel = 4;
  for (int y = 0; y < height; y++)
    memcpy((unsigned char*)locked_rect.pBits + locked_rect.Pitch * (size_t)y,
                           data + (width * (size_t)bytes_per_pixel) * y,
                           width * (size_t)bytes_per_pixel);
  texture->UnlockRect(0);
  
  if (hr == D3D_OK) {
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);

    *out_texture = texture;
    *out_width = (int)my_image_desc.Width;
    *out_height = (int)my_image_desc.Height;
    result = true;
  }

cleanup:
  stbi_image_free(data);
  return result;
}

bool ResManager::tryLoad(std::string ResourceName, std::string ResourceID) {
  auto& result = storage[ResourceName];
  if (result)
    return false; //Already exists

  if (ResourceID.empty()) {
    ImGuiIO &io = ImGui::GetIO();
    result.reset(new Texture(io.Fonts->TexID, io.Fonts->TexWidth, io.Fonts->TexHeight));
    return true;
  } else {
    PDIRECT3DTEXTURE9 texture;
    int w, h;
    if (LoadTextureFromFile("res/" + ResourceID, &texture, &w, &h)) {
      result.reset(new Texture(texture, w, h));
      return true;
    }
  }

  return false;
}

Texture *ResManager::get(std::string ResourceName) {
  auto result = storage.find(ResourceName);
  return result == storage.end() ? nullptr : result->second.get();
}
#include "ResourcesManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <d3d9.h>
#include "Shared.h"

ResManager Instance;

ResManager *ResManager::getInstance() { return &Instance; }

using namespace shared;

bool LoadTextureFromFile(std::string filename, PDIRECT3DTEXTURE9 *out_texture, int *out_width, int *out_height) {
  int width = 0;
  int height = 0;

  //Try to load the file into memory
  uint8_t *data = stbi_load(filename.c_str(), &width, &height, nullptr, 4); 
  if (!data)
    return false;

  bool result = false;
  PDIRECT3DTEXTURE9 texture;
  //Create an empty texture of appropriate size
  HRESULT hr = g_pd3dDevice->CreateTexture(width, height, 1,
                                           D3DUSAGE_DYNAMIC, D3DFMT_A8B8G8R8,
                                           D3DPOOL_DEFAULT, &texture, nullptr);

  if (hr != D3D_OK)
    goto cleanup;
  
  // Write the contents of the image into the texture
  D3DLOCKED_RECT locked_rect;
  if (texture->LockRect(0, &locked_rect, NULL, 0) != D3D_OK)
    goto cleanup;

  int bytes_per_pixel = 4;
  for (int y = 0; y < height; y++)
    memcpy((uint8_t *)locked_rect.pBits + locked_rect.Pitch * (size_t) y,
                      data + (width * (size_t) bytes_per_pixel) * y,
                      width * (size_t) bytes_per_pixel);
  texture->UnlockRect(0);
  
  if (hr == D3D_OK) {
    D3DSURFACE_DESC my_image_desc;
    texture->GetLevelDesc(0, &my_image_desc);

    *out_texture = texture;
    *out_width   = my_image_desc.Width;
    *out_height  = my_image_desc.Height;
    result = true;
  }

cleanup:
  stbi_image_free(data);
  return result;
}

Texture *ResManager::tryLoad(std::string ResourceName, std::string ResourceID) {
  auto& result = storage[ResourceName]; 
  if (result) // Already exists
    return result.get();

  if (!ResourceID.empty()) {
    PDIRECT3DTEXTURE9 texture;
    int w, h;
    if (LoadTextureFromFile(base_dir + ResourceID, &texture, &w, &h)) {
      result.reset(new Texture(texture, w, h));
      return result.get();
    }
  } else {  
    // Load default texture 
    //TODO: some appropriate hardcoded value for missing textures?
    ImGuiIO &io = ImGui::GetIO();
    result.reset(new Texture(io.Fonts->TexID, io.Fonts->TexWidth, io.Fonts->TexHeight));
    return result.get();
  }

  return nullptr;
}

Texture *ResManager::get(std::string ResourceName) {
  auto result = storage.find(ResourceName);
  return result == storage.end() ? nullptr : result->second.get();
}
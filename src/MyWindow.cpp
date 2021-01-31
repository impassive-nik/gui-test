#include "NativeWindow.h"
#include "ResourcesManager.h"
#include "Grid.h"

MyWindow::MyWindow(ResManager* mgr) : NativeWindow("MyGame"), res_mgr(mgr) {
  if (!res_mgr)
    res_mgr = ResManager::getInstance();
}

void MyWindow::run() {
  IM_ASSERT(ImGui::GetCurrentContext() != nullptr && "Missing dear imgui context. Refer to examples app!");

  static bool show_demo_window = false;
  static bool first_iteration = true;
  static Texture *Cell_tex[2];

  if (first_iteration) {
    first_iteration = false;
    res_mgr->tryLoad("FontTex", ""); // FIXME: it is wrong to initialize resources here. Move later
    Cell_tex[0] = res_mgr->tryLoad("CellA", "Cell_1.png");
    Cell_tex[1] = res_mgr->tryLoad("CellB", "Cell_2.png");
  }
  
  // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

  // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
  {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::Begin("MainWindow", nullptr, window_flags);

    static Grid<bool> my_grid = {5, 5, false};

    if (Cell_tex[0] && !my_grid.empty()) {
      int Cell_ID = 0;
      for (int y = 0; y < my_grid.getHeight(); ++y) {
        for (int x = 0; x < my_grid.getWidth(); ++x) {
          if (x > 0)
            ImGui::SameLine();

          ImGui::PushID(Cell_ID++);
          bool &val = my_grid.at(x, y);
          if (ImGui::ImageButton(Cell_tex[val]->ID, Cell_tex[val]->size))
            val = !val;
          ImGui::PopID();
        }
      }
    }

    ImGui::End();
  }
}
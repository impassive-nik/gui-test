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
  static Texture *Body_tex;

  if (first_iteration) {
    first_iteration = false;
    Cell_tex[0] = res_mgr->get(TexturesKind::Cell_1);
    Cell_tex[1] = res_mgr->get(TexturesKind::Cell_2);
    Body_tex    = res_mgr->tryLoad("Body", "Body.png");
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
    static int my_x = 0, my_y = 0;

    if (Cell_tex[0] && !my_grid.empty()) {
      int Cell_ID = 0;
      for (int y = 0; y < my_grid.getHeight(); ++y) {
        for (int x = 0; x < my_grid.getWidth(); ++x) {
          if (x > 0)
            ImGui::SameLine();

          auto last_x = ImGui::GetCursorPosX();

          ImGui::PushID(Cell_ID++);
          bool &val = my_grid.at(x, y);
          if (ImGui::ImageButton(Cell_tex[val]->ID, Cell_tex[val]->size)) {
            if (Body_tex && std::abs(my_x - x) + std::abs(my_y - y) == 1) {
              my_x = x;
              my_y = y;
            } else {
              val = !val;
            }
          }
          ImGui::PopID();

          if (Body_tex && x == my_x && y == my_y) {
            auto cur_x = ImGui::GetCursorPosX();
            ImGui::SameLine(last_x);
            ImGui::ImageButton(Body_tex->ID, Body_tex->size);
            ImGui::SetCursorPosX(cur_x);
          }
        }
      }
    }

    ImGui::End();
  }
}
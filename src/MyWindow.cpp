#include "NativeWindow.h"
#include "ResourcesManager.h"
#include "Grid.h"

MyWindow::MyWindow(ResManager* mgr) : NativeWindow("MyGame"), res_mgr(mgr) {
  if (!res_mgr)
    res_mgr = ResManager::getInstance();
}

enum class CellTy : int {
  EMPTY,
  WALL,
  ENEMY
};

void MyWindow::run() {
  IM_ASSERT(ImGui::GetCurrentContext() != nullptr && "Missing dear imgui context. Refer to examples app!");

  static bool show_demo_window = false;
  static bool first_iteration = true;
  static Texture *Cell_tex[3];
  static Texture *Buddy_tex;

  if (first_iteration) {
    first_iteration = false;
    Cell_tex[0] = res_mgr->get(TexturesKind::Cell_1);
    Cell_tex[1] = res_mgr->get(TexturesKind::Cell_2);
    Cell_tex[2] = res_mgr->get(TexturesKind::Enemy);
    Buddy_tex   = res_mgr->get(TexturesKind::Buddy);
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

    static Grid<CellTy> my_grid = {5, 5, CellTy::EMPTY};
    static int my_x = 0, my_y = 0;

    if (!my_grid.empty()) {
      int Cell_ID = 0;
      for (int y = 0; y < my_grid.getHeight(); ++y) {
        for (int x = 0; x < my_grid.getWidth(); ++x) {
          if (x > 0)
            ImGui::SameLine();

          auto last_x = ImGui::GetCursorPosX();

          ImGui::PushID(Cell_ID++);
          CellTy &val = my_grid.at(x, y);
          if (ImGui::ImageButton(Cell_tex[(int) val]->ID, Cell_tex[(int) val]->size)) {
            int dx = std::abs(my_x - x) + std::abs(my_y - y);
            if (dx == 1) {
              switch (val) {
              case CellTy::EMPTY:
                my_x = x;
                my_y = y;
                break;
              case CellTy::WALL:
                break;
              case CellTy::ENEMY:
                val = CellTy::EMPTY;
                break;
              }
            } else if (dx == 0) {
              if (my_x > 0 || my_y > 0) {
                val = CellTy::ENEMY;
                my_x = 0;
                my_y = 0;
              }
            } else {
              switch (val) {
              case CellTy::EMPTY:
                val = CellTy::WALL;
                break;
              case CellTy::WALL:
                val = CellTy::EMPTY;
                break;
              default:
                break;
              }
            }
          }
          ImGui::PopID();

          if (x == my_x && y == my_y) {
            auto cur_x = ImGui::GetCursorPosX();
            ImGui::SameLine(last_x);
            if (ImGui::ImageButton(Buddy_tex->ID, Buddy_tex->size)) {
              my_x = 0;
              my_y = 0;
            }
            ImGui::SetCursorPosX(cur_x);
          }
        }
      }
    }

    ImGui::End();
  }
}
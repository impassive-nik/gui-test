#include "NativeWindow.h"
#include "ResourcesManager.h"

MyWindow::MyWindow(ResManager* mgr) : NativeWindow("MyGame"), res_mgr(mgr) {
  if (!res_mgr)
    res_mgr = ResManager::getInstance();
}

void MyWindow::run() {
  IM_ASSERT(ImGui::GetCurrentContext() != nullptr && "Missing dear imgui context. Refer to examples app!");

  static bool show_demo_window = false;
  static bool first_iteration = true;

  if (first_iteration) {
    first_iteration = false;
    res_mgr->tryLoad("FontTex", ""); // FIXME: it is wrong to initialize resources here. Move later
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

    ImGui::Text("This is some useful text.");

    ImGui::ColorEdit3("clear color", (float*)&bg_color);

    if (auto img = res_mgr->get("FontTex")) {
      if (ImGui::ImageButton(img->ID, img->size)) {
        show_demo_window = true;
      }
    } else {
      ImGui::Text("Couldn't load the texture");
    }

    ImGui::End();
  }
}
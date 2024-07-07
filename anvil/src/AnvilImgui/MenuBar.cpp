#include "AnvilImgui/MenuBar.h"

#include "application.h"
#include "imgui.h"
#include "components/Sprite2DComponent.h"
#include "components/Transform2DComponent.h"
#include "states/editstate.h"

namespace anvil {
	
    void MenuBar::draw() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Imgui Demo", "Ctrl+W")) {
                    show_demo_window = !show_demo_window;
                }
                if (ImGui::MenuItem("Editor", "Ctrl+W")) {
                    Application::Instance()->getStateMachine()->changeState(new EditState);
                }
                if (ImGui::MenuItem("debug view")) {
                    showInspector = !showInspector;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        showInspector ? ImguiSystem::Instance()->ShowWidget("SceneWidget")
                      : ImguiSystem::Instance()->HideWidget("SceneWidget");
    }
}

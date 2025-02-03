#include "ImguiSystem.h"
#include "application.h"
#include <imgui.h>

#include "MenuBar.h"
#include "SceneWidget.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "backends/imgui_impl_sdl3.h"
#include "components/Sprite2DComponent.h"
#include "states/editstate.h"

namespace anvil {

    void ImguiSystem::init(std::unique_ptr<Window> &window, SDL_Renderer *renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io{ImGui::GetIO()};

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplSDL3_InitForSDLRenderer(window->getWindow(), renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);

        RegisterWidget("MenuBar", std::make_shared<MenuBar>());
        auto sceneWidget = std::make_shared<GameSceneWidget>();
        RegisterWidget("SceneWidget", sceneWidget);
        if (scene_) {
            sceneWidget->setRootNode(scene_);
        }
    }

    static ImguiSystem *m_instance = nullptr;

    ImguiSystem *ImguiSystem::Instance() {
        if (m_instance == nullptr) {
            m_instance = new ImguiSystem();
            return m_instance;
        }
        return m_instance;
    }

    void ImguiSystem::render() {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        for (const auto& pair : widgets) {
            if (widgetVisibility[pair.first]) {
                pair.second->draw();
            }
        }
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImguiSystem::update() {
        for (const auto& task : updateTasks) {
            task();
        }
        updateTasks.clear();

    }

    void ImguiSystem::shutDown() {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void ImguiSystem::RegisterWidget(const std::string& name, std::shared_ptr<IWidget> widget) {
        widgets[name] = widget;
        widgetVisibility[name] = false;
    }
    void ImguiSystem::UnregisterWidget(const std::string& name) {
        widgets.erase(name);
        widgetVisibility.erase(name);
    }
    void ImguiSystem::ShowWidget(const std::string& name) {
        if (widgetVisibility.find(name) != widgetVisibility.end()) {
            widgetVisibility[name] = true;
        }
    }
    void ImguiSystem::HideWidget(const std::string& name) {
        if (widgetVisibility.find(name) != widgetVisibility.end()) {
            widgetVisibility[name] = false;
        }
    }

    void ImguiSystem::addUpdateTask(std::function<void()> task) {
        updateTasks.push_back(task);
    }

    void ImguiSystem::setScene(std::shared_ptr<GameObject> scene) {
        auto it = widgets.find("SceneWidget");
        if (it != widgets.end()) {
            auto sceneWidget = std::dynamic_pointer_cast<GameSceneWidget>(it->second);
            sceneWidget->setRootNode(scene);
        }
    }

    std::shared_ptr<GameObject> ImguiSystem::getScene() {
        return scene_;
    }
}

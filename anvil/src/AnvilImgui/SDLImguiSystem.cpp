#include "SDLImguiSystem.h"
#include "application.h"
#include "Render/SDL/SDL_Renderer.hpp"
#include <imgui.h>

#include "MenuBar.h"
#include "SceneWidget.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "backends/imgui_impl_sdl3.h"
#include "states/editstate.h"

namespace anvil {

    void SDLImguiSystem::init(SDL_Window* window, std::shared_ptr<IRenderer> renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io{ImGui::GetIO()};

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        SDL_Renderer* sdlRenderer = static_cast<SDLRenderer*>(renderer.get())->getRawRenderer();
        if (auto sdlRenderer = dynamic_cast<SDLRenderer*>(renderer.get())) {
            SDL_Renderer* raw = sdlRenderer->getRawRenderer();
            ImGui_ImplSDL3_InitForSDLRenderer(window, raw);
            ImGui_ImplSDLRenderer3_Init(raw);
        }
        else {
            Logger::Err("SDLImguiSystem: Renderer is not SDLRenderer.");
            return;
        }

        RegisterWidget("MenuBar", std::make_shared<MenuBar>());
    }

    static SDLImguiSystem *m_instance = nullptr;

    SDLImguiSystem *SDLImguiSystem::Instance() {
        if (m_instance == nullptr) {
            m_instance = new SDLImguiSystem();
            return m_instance;
        }
        return m_instance;
    }

    void SDLImguiSystem::render() {
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

    void SDLImguiSystem::handleEvent(SDL_Event& event) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        ImGuiIO& io = ImGui::GetIO();
        float mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    }

    void SDLImguiSystem::update() {
        for (const auto& task : updateTasks) {
            task();
        }
        updateTasks.clear();

    }

    void SDLImguiSystem::shutDown() {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void SDLImguiSystem::RegisterWidget(const std::string& name, std::shared_ptr<IWidget> widget) {
        widgets[name] = widget;
        widgetVisibility[name] = false;
    }
    void SDLImguiSystem::UnregisterWidget(const std::string& name) {
        widgets.erase(name);
        widgetVisibility.erase(name);
    }
    void SDLImguiSystem::ShowWidget(const std::string& name) {
        if (widgetVisibility.find(name) != widgetVisibility.end()) {
            widgetVisibility[name] = true;
        }
    }
    void SDLImguiSystem::HideWidget(const std::string& name) {
        if (widgetVisibility.find(name) != widgetVisibility.end()) {
            widgetVisibility[name] = false;
        }
    }

    void SDLImguiSystem::addUpdateTask(std::function<void()> task) {
        updateTasks.push_back(task);
    }

}

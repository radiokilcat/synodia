#pragma once

#include <imgui.h>
#include <vector>
#include <queue>
#include "backends/imgui_impl_sdlrenderer3.h"
#include "backends/imgui_impl_sdl3.h"
#include "window.h"
#include "game_objects/gameobject.h"

namespace anvil {
    class GameObject;

class ImguiSystem {
public:
    ImguiSystem();
    ~ImguiSystem();

    void init(std::unique_ptr<anvil::Window>& window, SDL_Renderer* renderer);
    static ImguiSystem* Instance();

    void drawMenuBar();
    void drawWidgets();
    void render();
    void shutDown();
    void addGameObject(GameObject* obj);
    void setRootNode(std::shared_ptr<GameObject> node);
    void drawPropertiesWidget(std::shared_ptr<GameObject> node);

    void drawSingleNode(std::shared_ptr<GameObject> node);
    void drawNode(std::weak_ptr<GameObject> node);

private:
    bool showInspector = true;
    std::vector<GameObject*> objects;
    std::queue<std::function<void()>> callbacks;
    std::shared_ptr<GameObject> scene;
    std::shared_ptr<GameObject> currentObj;
    bool show_demo_window = false;
};

}

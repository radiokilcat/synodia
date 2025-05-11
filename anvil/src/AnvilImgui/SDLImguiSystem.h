#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <unordered_map>
#include "window.h"
#include "ImguiSystem.h"

namespace anvil {

class SDLImguiSystem : public ImguiSystem {
public:
    SDLImguiSystem() = default;
    ~SDLImguiSystem() = default;
    void init(SDL_Window* window, std::shared_ptr<IRenderer> renderer);
    static SDLImguiSystem* Instance();

    void update();
    void render();
    void shutDown();
    void handleEvent(SDL_Event& event) ;

    void RegisterWidget(const std::string& name, std::shared_ptr<IWidget> widget);
    void UnregisterWidget(const std::string& name);
    void ShowWidget(const std::string& name);
    void HideWidget(const std::string& name);
    void addUpdateTask(std::function<void()> task);

private:
    std::vector<std::function<void()>> updateTasks;
    bool showInspector = true;
    std::unordered_map<std::string, std::shared_ptr<IWidget>> widgets;
    std::unordered_map<std::string, bool> widgetVisibility;
    bool show_demo_window = false;
};

}

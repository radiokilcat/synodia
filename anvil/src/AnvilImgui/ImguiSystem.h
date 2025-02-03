#pragma once

#include <vector>
#include <queue>
#include "window.h"
#include "game_objects/gameobject.h"

namespace anvil {
    class GameObject;

    class IWidget {
    public:
        virtual ~IWidget() = default;
        virtual void draw() = 0;
    };

class ImguiSystem {
public:
    ImguiSystem() = default;
    ~ImguiSystem() = default;

    void init(std::unique_ptr<anvil::Window>& window, SDL_Renderer* renderer);
    static ImguiSystem* Instance();

    void render();
    void update();
    void shutDown();

    void RegisterWidget(const std::string& name, std::shared_ptr<IWidget> widget);
    void UnregisterWidget(const std::string& name);
    void ShowWidget(const std::string& name);
    void HideWidget(const std::string& name);
    void addUpdateTask(std::function<void()> task);
    

    void setScene(std::shared_ptr<GameObject> scene);
    std::shared_ptr<GameObject> getScene();

private:
    std::vector<std::function<void()>> updateTasks;
    bool showInspector = true;
    std::vector<GameObject*> objects;
    std::shared_ptr<GameObject> scene_;
    std::shared_ptr<GameObject> currentObj;
    std::unordered_map<std::string, std::shared_ptr<IWidget>> widgets;
    std::unordered_map<std::string, bool> widgetVisibility;
    bool show_demo_window = false;
};

}

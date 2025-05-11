#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <unordered_map>
#include "window.h"
#include "../Render/IRenderer.hpp"
#include "../Logger/Logger.h"

namespace anvil {

    class IWidget {
    public:
        virtual ~IWidget() = default;
        virtual void draw() = 0;
    };

    class ImguiSystem {
    public:
        virtual void init(SDL_Window* window, std::shared_ptr<IRenderer> renderer) = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void shutDown() = 0;
        virtual void handleEvent(SDL_Event& event) = 0;

        virtual void RegisterWidget(const std::string& name, std::shared_ptr<IWidget> widget) = 0;
        virtual void UnregisterWidget(const std::string& name) = 0;
        virtual void ShowWidget(const std::string& name) = 0;
        virtual void HideWidget(const std::string& name) = 0;
        virtual void addUpdateTask(std::function<void()> task) = 0;
    };

}

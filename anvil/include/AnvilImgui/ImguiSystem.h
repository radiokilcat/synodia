#pragma once

#include <imgui.h>
#include "backends/imgui_impl_sdlrenderer3.h"
#include "backends/imgui_impl_sdl3.h"
#include "window.h"

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

private:
};


#pragma once

#include <SDL3/SDL.h>

#include <memory>

#include "window.h"

namespace anvil {

class Renderer
{
public:
    static std::unique_ptr<Renderer> create(Window& window);

    Renderer(Window& window);
    ~Renderer();

private:
    void init();
    void quit();

    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_renderer;
//    SDL_Renderer* m_renderer;
};

}


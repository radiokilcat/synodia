#pragma once

#include <SDL3/SDL.h>
#include <memory>

#include "window.h"

namespace anvil {

class Renderer
{
public:
    static std::shared_ptr<Renderer> create(std::unique_ptr<Window>& window);

    static std::shared_ptr<Renderer> create(SDL_Window *window);

    Renderer(SDL_Window* window);
    ~Renderer();

    [[nodiscard]] auto getRenderer() const noexcept -> SDL_Renderer* { return m_renderer.get(); }

private:
    void init();
    void quit();

    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_renderer;
};

}


#include "renderer.h"


namespace anvil {

std::shared_ptr<Renderer> Renderer::create(SDL_Window* window) {
    return std::make_unique<Renderer>(window);
}

Renderer::Renderer(SDL_Window* window)
    : m_renderer(SDL_CreateRenderer(window, nullptr, 0), &SDL_DestroyRenderer)
{
}

Renderer::~Renderer() {}

void init();
void quit();

}


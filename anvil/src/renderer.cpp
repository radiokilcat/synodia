#include "renderer.h"


namespace anvil {

std::unique_ptr<Renderer> Renderer::create(Window& window)
{
    return std::make_unique<Renderer>(window);
}

Renderer::Renderer(Window& window)
    : m_renderer(SDL_CreateRenderer(window.get(), " ", 0), &SDL_DestroyRenderer)
{
}

Renderer::~Renderer() {}

void init();
void quit();

}


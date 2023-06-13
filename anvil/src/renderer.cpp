#include "renderer.h"


namespace anvil {

//std::unique_ptr<Renderer> Renderer::create(Window& window)
std::shared_ptr<Renderer> Renderer::create(std::unique_ptr<Window>& window)
{
    return std::make_unique<Renderer>(window);
}

Renderer::Renderer(std::unique_ptr<Window>& window)
    : m_renderer(SDL_CreateRenderer(window.get()->getWindow(), nullptr, 0), &SDL_DestroyRenderer)
{
}


Renderer::~Renderer() {}

void init();
void quit();

}


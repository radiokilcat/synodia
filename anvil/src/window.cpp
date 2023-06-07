#include <stdexcept>

#include "window.h"

namespace anvil {

std::unique_ptr<Window> Window::create(const std::string &title,
                                       int width,
                                       int height,
                                       SDL_WindowFlags flags)
{
    return std::make_unique<Window>(title, width, height, flags);
}

Window::Window(const std::string &title, int width, int height, SDL_WindowFlags flags)
    :m_window(SDL_CreateWindow(title.c_str(), width, height, flags), &SDL_DestroyWindow)
{
    if (!m_window) {
        throw std::runtime_error(SDL_GetError());
    }
}

Window::~Window()
{

}


std::optional<SDL_Event> Window::pollEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        return event;
    }
    else {
        return std::nullopt;
    }
}

void Window::init()
{
    if (SDL_InitSubSystem(SDL_InitFlags::SDL_INIT_VIDEO)) {
        SDL_LogCritical(SDL_LogCategory::SDL_LOG_CATEGORY_ERROR, SDL_GetError());
    }
}

void Window::quit()
{
    SDL_QuitSubSystem(SDL_InitFlags::SDL_INIT_VIDEO);
}

}

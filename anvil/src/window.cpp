#include <stdexcept>

#include "window.h"

namespace anvil {

Window::Window(const std::string &title, int width, int height, SDL_WindowFlags flags)
    :m_window(SDL_CreateWindow(title.c_str(), width, height, flags), &SDL_DestroyWindow)
{
    if (!m_window)
    {
        throw std::runtime_error(SDL_GetError());
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

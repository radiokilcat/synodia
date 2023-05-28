
#include "window.h"

Window::Window(const std::string &title, int width, int height, SDL_WindowFlags flags)
    : m_window(SDL_CreateWindow(title.c_str(), width, height, flags), &SDL_DestroyWindow)
{
    SDL_assert(m_window);
}

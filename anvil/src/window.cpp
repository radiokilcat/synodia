#include <stdexcept>

#include "window.h"
#include "Logger/Logger.h"

namespace anvil {

std::unique_ptr<Window> Window::create(const std::string &title,
                                       int width,
                                       int height,
                                       SDL_WindowFlags flags)
{
    Logger::Log("Window created with title: " + title + " width: " + std::to_string(width) + " height: " + std::to_string(height));
    return std::make_unique<Window>(title, width, height, flags);
}

Window::Window(const std::string &title, int width, int height, SDL_WindowFlags flags)
    :m_window(SDL_CreateWindow(title.c_str(), width, height, flags), &SDL_DestroyWindow)
{
    if (!m_window) {
        throw std::runtime_error(SDL_GetError());
    }
    {
        SDL_DisplayID display = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);

        if (mode) {
            SDL_Log("Display %" SDL_PRIu32 " mode %d: %dx%d@%gx %gHz\n",
                    display, mode->w, mode->h, mode->pixel_density, mode->refresh_rate);
        }
        // width = mode->w;
        // height = mode->h;
    }
}

int Window::getWidth() const {
    // int w, h;
    // window->getSize(&w, &h);
    return 0;
}

int Window::getHeight() const {
    // int w, h;
    // window->getSize(&w, &h);
    return 0;
}

Window::~Window() {
    quit();
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

std::pair<int, int> Window::getWindowSize()
{
    int width = 0;
    int height = 0;
    int result = SDL_GetWindowSize(getWindow(), &width, &height);

    if (result != 0)
        SDL_LogCritical(SDL_LogCategory::SDL_LOG_CATEGORY_ERROR, SDL_GetError());

    return std::pair<int, int>(width, height);

}

void Window::quit()
{
    SDL_QuitSubSystem(SDL_InitFlags::SDL_INIT_VIDEO);
}

}

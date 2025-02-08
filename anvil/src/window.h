#pragma once

#include <string>
#include <memory>
#include <optional>

#include "SDL3/SDL.h"

namespace anvil {

class Window
{
public:
    static std::unique_ptr<Window> create(const std::string &title,
                                          int width,
                                          int height,
                                          SDL_WindowFlags flags = (SDL_WindowFlags)0);

    Window(const std::string& title, int width = 800, int height = 600,
                    SDL_WindowFlags flags = (SDL_WindowFlags)0);
    ~Window();

    void init();
    std::pair<int, int> getWindowSize();

    [[nodiscard]] auto getWindow() const noexcept -> SDL_Window* { return m_window.get(); }
    std::optional<SDL_Event> pollEvents();
    int getWidth() const;
    int getHeight() const;

private:
    int width;
    int height;

    void quit();
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
};

}

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

//    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>getWindow() { return m_window; }
//    SDL_Window& getWindow() { return m_window; }
    [[nodiscard]] auto get() const noexcept -> SDL_Window* { return m_window.get(); }
//    SDL_Window* getWindow() { return m_window; }
    std::optional<SDL_Event> pollEvents();

private:


    void init();
    void quit();

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
//    SDL_Window* m_window;
};

}

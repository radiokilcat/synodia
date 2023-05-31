#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>

#include "SDL3/SDL.h"

namespace anvil {

class Window
{
public:
    explicit Window(const std::string& title, int width, int height, SDL_WindowFlags flags);

    static void init();
    static void quit();
private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
};

}
#endif // WINDOW_H

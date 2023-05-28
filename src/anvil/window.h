#ifndef WINDOW_H
#define WINDOW_H

#define SDL_MAIN_HANDLED

#include <memory>
#include <string>
#include "SDL3/SDL.h"

#ifdef EXPORT_DLL
#  define __declspec(dllexport) Window
#else
#  define __declspec(dllimport) Window
#endif


class Window
{
public:
    Window(const std::string& title, int width, int height, SDL_WindowFlags flags = (SDL_WindowFlags)0);
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
};

#endif // WINDOW_H

#include <iostream>
#include "window.h"
#include "application.h"



int main(int argc, char *argv[])
{
    auto win = anvil::Window::create("SDL_test", 600, 800);
    auto app = anvil::Application::create(std::move(win));
    app->handle_loop();


//    SDL_Init(SDL_INIT_VIDEO);

//    SDL_Window *window = SDL_CreateWindow(
//        "SDL2Test",
//        640,
//        480,
//        0
//        );

//    SDL_Renderer *renderer = SDL_CreateRenderer(window, " ", 0);
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//    SDL_RenderClear(renderer);
//    SDL_RenderPresent(renderer);

//    SDL_Delay(3000);

//    SDL_DestroyWindow(window);
//    SDL_Quit();

    return 0;
}

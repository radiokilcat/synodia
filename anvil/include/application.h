#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

namespace anvil {

class Window;
class Renderer;

struct GameSettings {
    void validate();

    int screenWidth = 256;
    int screenHeight = 224;
    int screenScale = 3;

    std::string windowTitle = "Game";
};

class Application
{
public:
    static std::unique_ptr<Application> create(GameSettings settings);
    Application(GameSettings settings);
    ~Application();

    void run();


private:
    void init();
    void main_loop();
    void update();
    void render();
    void cleanup();

    std::unique_ptr<Window> m_window = nullptr;
    std::unique_ptr<Renderer> m_renderer = nullptr;
    //ToDo: make SDL_Texture with it's own wrapper class
    SDL_Texture* screenTexture = nullptr;

    GameSettings m_settings;
    bool running;
};

}

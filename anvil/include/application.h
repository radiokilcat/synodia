#pragma once

#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#include <SDL3/SDL.h>

namespace anvil {

class Window;
class Renderer;
class TextureManager;
class GameObject;

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
    std::shared_ptr<Renderer> m_renderer = nullptr;
    //ToDo: make SDL_Texture with it's own wrapper class
    SDL_Texture* screenTexture = nullptr;
    TextureManager* m_textureManager = nullptr;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    GameSettings m_settings;
    std::filesystem::path m_resPath;
    bool running;

};

}

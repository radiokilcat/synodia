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

    /*GameSettings& operator=(const GameSettings& other) {
        if (&other == this) {
            return *this;
        }
    }*/

    void validate();

    int screenWidth = 256;
    int screenHeight = 224;
    int screenScale = 3;

    std::string windowTitle = "Game";
};

/*
    SDLGetTicks return unproper ticks - its milliseconds since application
    have started.
*/
struct GameTime {
    long long m_previousFrameMs = 0;
    long long m_currentFrameMs = 0;
    long long m_deltaTimeMs = 0;
};
class Application
{
public:
    static Application* Instance();

    void init(const GameSettings& settings);
    void run();
    void quit();

    void addGameObject(std::unique_ptr<GameObject> gameObject);
    std::shared_ptr<Renderer> getRenderer() const;
    int getScreenWidth();
    int getScreenHeight();

    Application();
    ~Application();

private:
    void main_loop();
    void update();
    void render();
    void cleanup();

    std::unique_ptr<Window> m_window = nullptr;
    std::shared_ptr<Renderer> m_renderer = nullptr;
    SDL_Texture* screenTexture = nullptr;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    GameSettings m_settings;
    std::filesystem::path m_resPath;
    bool m_running;
    GameTime m_gameTime;
};

}

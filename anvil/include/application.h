#pragma once

#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <functional>

#include <SDL3/SDL.h>
#include "AnvilImgui/ImguiSystem.h"

namespace anvil {

class Window;
class Renderer;
class TextureManager;
class GameObject;
class GameStateMachine;
class Logger;

struct GameSettings {

    void validate();

    int screenWidth = 256;
    int screenHeight = 224;
    int screenScale = 3;
    int FPS = 60;

    std::string windowTitle = "Game";
};


class Application
{
public:
    static Application* Instance();

    void init(const GameSettings& settings);
    void run();
    void quit();

    std::shared_ptr<Renderer> getRenderer() const;
    int getScreenWidth();
    int getScreenHeight();

    Uint64 getTicks();

    Application();
    ~Application();

    GameStateMachine* getStateMachine() const;
    void setStateMachine(GameStateMachine* newStateMachine);
    void addInitCallback(std::function<void()> callback);
    void addUpdateCallback(std::function<void()> callback);

private:
    void main_loop();
    void update();
    void render();
    void cleanup();

    std::unique_ptr<Window> m_window = nullptr;
    std::shared_ptr<Renderer> m_renderer = nullptr;

    SDL_Texture* screenTexture = nullptr;
    GameStateMachine* m_stateMachine;

    std::function<void()> m_updateCallback;
    std::function<void()> m_initCallback;

    GameSettings m_settings;
    std::filesystem::path m_resPath;
    bool m_running;
};

}

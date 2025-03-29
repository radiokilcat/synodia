#pragma once

#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <functional>

#include <SDL3/SDL.h>
#include "AssetStore/AssetStore.h"
#include "EventBus/EventBus.h"
#include "ECS/ECS.h"
#include "AnvilImgui/ImguiSystem.h"
#include "AppSettings.hpp"

namespace anvil {

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Window;
class Renderer;
class TextureManager;
class GameObject;
class GameStateMachine;
class Logger;
class AppSettings;

class Application
{
public:
    static Application* Instance();

    void init(AppSettings settings);
    void run();
    void quit();

    SDL_Renderer* getRenderer() const;
    int getScreenWidth();
    int getScreenHeight();
    int getLogicalWidth();
    int getLogicalHeight();
    int getMapWidth();
    int getMapHeight();

    Uint64 getTicks();

    void Setup();

void ProcessInput();

    Application();
    ~Application();

    GameStateMachine* getStateMachine() const;
    void setStateMachine(GameStateMachine* newStateMachine);
    void addInitCallback(std::function<void()> callback);
    void addUpdateCallback(std::function<void()> callback);


private:
    void update();
    void render();
    void cleanup();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::unique_ptr<Window> m_window = nullptr;
    std::shared_ptr<Renderer> m_renderer = nullptr;

    SDL_Texture* screenTexture = nullptr;
    GameStateMachine* m_stateMachine;
    SDL_Rect camera;

    AppSettings m_settings;
    std::filesystem::path m_resPath;

    bool isRunning;
    bool isDebug;
    int millisecsPreviousFrame = 0;
    int mapWidth;
    int mapHeight;
    int windowWidth;
    int windowHeight;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;

    std::function<void()> m_updateCallback;
    std::function<void()> m_initCallback;
};

}

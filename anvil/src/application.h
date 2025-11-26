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
class IRenderer;
class TextureManager;
class GameObject;
class GameStateMachine;
class Logger;
struct AppSettings;

class Application
{
public:
    Application();
    ~Application();

    static Application* Instance();

    void init(AppSettings settings);
    void run();
    void quit();

    std::shared_ptr<IRenderer> getRenderer() const;
    std::shared_ptr<ImguiSystem> getImguiSystem() const;
    GameStateMachine* getStateMachine() const;
    Uint64 getTicks();

    void setStateMachine(GameStateMachine* newStateMachine);
    void addInitCallback(std::function<void()> callback);
    void addUpdateCallback(std::function<void()> callback);

    int getScreenWidth();
    int getScreenHeight();
    int getLogicalWidth();
    int getLogicalHeight();
    int getMapWidth();
    int getMapHeight();

private:
    void update();
    void render();
    void cleanup();
    void Setup();
    void ProcessInput();

    SDL_Window* window = nullptr;
    std::shared_ptr<IRenderer> renderer = nullptr;
    std::unique_ptr<Window> m_window = nullptr;
    std::shared_ptr<ImguiSystem> imgui = nullptr;

    SDL_Texture* screenTexture = nullptr;
    GameStateMachine* m_stateMachine;
    SDL_Rect camera;

    AppSettings m_settings;
    std::filesystem::path m_resPath;

    bool isRunning;
    bool isDebug;
    Uint64 millisecsPreviousFrame = 0;
    int mapWidth;
    int mapHeight;
    int windowWidth;
    int windowHeight;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;

    std::vector<std::function<void()>> updateCallbacks;
    std::function<void()> m_updateCallback;
    std::function<void()> m_initCallback;
};

}

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
class GameStateMachine;

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

    void addGameObject(std::unique_ptr<GameObject> gameObject);
    std::shared_ptr<Renderer> getRenderer() const;
    int getScreenWidth();
    int getScreenHeight();

    Uint32 getTicks();

    Application();
    ~Application();

    GameStateMachine* getStateMachine() const;
    void setStateMachine(GameStateMachine *newStateMachine);

private:
    void main_loop();
    void update();
    void render();
    void cleanup();

    std::unique_ptr<Window> m_window = nullptr;
    std::shared_ptr<Renderer> m_renderer = nullptr;
    SDL_Texture* screenTexture = nullptr;
    GameStateMachine* m_stateMachine;

    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    GameSettings m_settings;
    std::filesystem::path m_resPath;
    bool m_running;
};

}

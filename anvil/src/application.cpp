#include "application.h"
#include "window.h"
#include "renderer.h"
#include "texturemanager.h"
#include "gameobject.h"
#include "utils.h"
#include "inputhandler.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>
#include <filesystem>
#include <iostream>


SDL_Texture* createBlankTexture(SDL_Renderer* renderer, int width, int height)
{
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET, width, height);
    if (!texture) {
        printf("Unable to create streamable blank texture! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }
    return texture;
}

namespace anvil {

void GameSettings::validate()
{
    assert(screenWidth > 0);
    assert(screenHeight > 0);
    assert(screenScale > 0);
}

static Application* m_instance = nullptr;

Application *Application::Instance()
{
    if(m_instance == 0)
    {
        m_instance = new Application();
        return m_instance;
    }
    return m_instance;

}

Application::Application() : m_running(true)
{
}

Application::~Application()
{

}

void Application::run()
{
    main_loop();
    cleanup();
}

void Application::quit()
{
    m_running = false;
}

void Application::addGameObject(std::unique_ptr<GameObject> gameObject)
{
    m_gameObjects.push_back(std::move(gameObject));
}

std::shared_ptr<Renderer> Application::getRenderer() const
{
    std::cout << "update game objects " <<  std::endl;
    return m_renderer;
}

void Application::init(const GameSettings& settings)
{
    m_settings = settings;

    std::filesystem::current_path(getExecutableDir());
    m_resPath = std::filesystem::current_path().parent_path() / "res";
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        std::exit(1);
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        std::exit(1);
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        std::exit(1);
    }

    m_window = Window::create(m_settings.windowTitle.c_str(),
                              m_settings.screenWidth * m_settings.screenScale,
                              m_settings.screenHeight * m_settings.screenScale);
    m_renderer = Renderer::create(m_window);

    SDL_SetRenderDrawBlendMode(m_renderer->getRenderer(), SDL_BLENDMODE_BLEND);

    // bg color
    SDL_SetRenderDrawColor(m_renderer->getRenderer(), 100, 149, 237, SDL_ALPHA_OPAQUE);
}

void Application::main_loop()
{
    while (m_running)
    {
        m_gameTime.m_currentFrameMs = SDL_GetTicks();
        InputHandler::instance()->handleEvents();
        update();
        render();
        m_gameTime.m_previousFrameMs = m_gameTime.m_currentFrameMs;
    }
}

void Application::update()
{
    m_gameTime.m_deltaTimeMs = m_gameTime.m_currentFrameMs - m_gameTime.m_previousFrameMs;
    for (const auto& it: m_gameObjects)
    {
        it->update(m_gameTime);
    }
}

void Application::render()
{
    SDL_RenderClear(m_renderer->getRenderer());
    SDL_SetRenderTarget(m_renderer->getRenderer(), nullptr);

    for (const auto& gameObject: m_gameObjects)
    {
        gameObject->draw(m_renderer);
    }

    SDL_RenderPresent(m_renderer->getRenderer());
}

void Application::cleanup()
{
    InputHandler::instance()->clean();
}

}

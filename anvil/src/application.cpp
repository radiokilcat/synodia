#include "application.h"
#include "window.h"
#include "renderer.h"
#include "inputhandler.h"
#include "game_state_machine.h"
#include "audio_manager.h"
#include "utils.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>
#include <iostream>
#include <ui/scrollable_text.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>

#include "logger.h"

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

Application* Application::Instance()
{
    if(m_instance == nullptr)
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
    anvil::ScrollableText::registerWithFactory();

    main_loop();
    cleanup();
}

void Application::quit()
{
    m_running = false;
}

std::shared_ptr<Renderer> Application::getRenderer() const
{
    return m_renderer;
}

int Application::getScreenWidth()
{
    return m_window->getWindowSize().first;
}

int Application::getScreenHeight()
{
    return m_window->getWindowSize().second;
}

Uint64 Application::getTicks()
{
    return SDL_GetTicks();
}

void Application::init(const GameSettings& settings)
{
    Logger::instance().startTimer();
    m_settings = settings;

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
    AudioManager::instance().initAudio();

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        std::exit(1);
    }

    m_window = Window::create(m_settings.windowTitle.c_str(),
                              m_settings.screenWidth * m_settings.screenScale,
                              m_settings.screenHeight * m_settings.screenScale);


    m_renderer = Renderer::create(m_window);

    m_stateMachine = new GameStateMachine();


    if (m_initCallback) {
        m_initCallback();
    }

#ifndef NDEBUG
    ImguiSystem::Instance()->init(m_window, m_renderer->getRenderer());
#endif
    SDL_SetRenderDrawBlendMode(m_renderer->getRenderer(), SDL_BLENDMODE_BLEND);

    // bg color
    SDL_SetRenderDrawColor(m_renderer->getRenderer(), 100, 149, 237, SDL_ALPHA_OPAQUE);
    Logger::instance().endTimer("application initialization");
}

void Application::main_loop()
{
    const int DELAY_TIME = 1000.0f / m_settings.FPS;

    while (m_running)
    {
        Uint32 frameStart, frameTime;
        frameStart = SDL_GetTicks();

        InputHandler::instance()->handleEvents();

        update();
        render();

        frameTime = getTicks() - frameStart;
        if(frameTime < DELAY_TIME)
        {
            SDL_Delay((int)(DELAY_TIME - frameTime));
        }
    }
}

void Application::update()
{
    if (m_updateCallback) {
        m_updateCallback();
    }
    m_stateMachine->update();
}

void Application::render()
{
#ifndef NDEBUG
    ImguiSystem::Instance()->drawMenuBar(m_window);
#endif

    SDL_RenderClear(m_renderer->getRenderer());
    SDL_SetRenderTarget(m_renderer->getRenderer(), nullptr);

    m_stateMachine->render();

#ifndef NDEBUG
    ImguiSystem::Instance()->render();
#endif

    SDL_RenderPresent(m_renderer->getRenderer());
}

void Application::cleanup()
{
    InputHandler::instance()->clean();
    AudioManager::instance().cleanup();
#ifndef NDEBUG
    ImguiSystem::Instance()->shutDown();
#endif

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}

GameStateMachine* Application::getStateMachine() const
{
    return m_stateMachine;
}

void Application::addInitCallback(std::function<void ()> callback)
{
    m_initCallback = std::move(callback);

}

void Application::addUpdateCallback(std::function<void ()> callback)
{
    m_updateCallback = std::move(callback);
}

}

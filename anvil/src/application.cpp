#include "application.h"
#include "AppSettings.hpp"
#include "window.h"
#include "Render/IRenderer.hpp"
#include "Render/SDL/SDL_Renderer.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "inputhandler.h"
#include "states/game_state_machine.h"
#include "states/menustate.h"
#include "states/playstate.h"
#include "audio_manager.h"
#include "utils.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>
#include <AnvilImgui/ImguiFactory.h>
#include <AnvilImgui/SceneWidget.h>
#include "backends/imgui_impl_sdlrenderer3.h"
#include "backends/imgui_impl_sdl3.h"

#include "Logger/Logger.h"


namespace anvil {

static Application* m_instance = nullptr;

Application* Application::Instance() {
    if(m_instance == nullptr) {
        m_instance = new Application();
        return m_instance;
    }
    return m_instance;
}

Application::Application()
    : isRunning(true)
    , isDebug(false)
    , registry(std::make_unique<Registry>())
    , assetStore(std::make_unique<AssetStore>())
    , eventBus(std::make_unique<EventBus>()) {

    Logger::Log("Application constructor called!");
}

Application::~Application() {
    Logger::Log("Game destructor called!");   
}

void Application::init(AppSettings settings) {
    m_settings = settings;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_WEBP;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        std::exit(1);
    }
    // AudioManager::instance().initAudio();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        std::exit(1);
    }

    if (TTF_Init() != 0) {
        Logger::Err("Error initializing SDL TTF.");
        return;
    }

    {
        Uint32 windowFlags = SDL_WINDOW_BORDERLESS;
        
        if (m_settings.rendererType == RendererType::OpenGL) {
            windowFlags |= SDL_WINDOW_OPENGL;
        }

        #ifndef NDEBUG
            windowFlags |= SDL_WINDOW_RESIZABLE;
        #endif

        window = SDL_CreateWindow(m_settings.windowTitle.c_str(), m_settings.screenWidth,
        m_settings.screenHeight, windowFlags);
        SDL_DisplayID display = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);

        if (mode) {
            SDL_Log("Display %" SDL_PRIu32 " mode %d: %dx%d@%gx %gHz\n",
                    display, mode->w, mode->h, mode->pixel_density, mode->refresh_rate);
        }
    }

    {
        if (m_settings.rendererType == RendererType::SDL) {
            renderer = std::make_shared<SDLRenderer>();
        } else if (m_settings.rendererType == RendererType::OpenGL) {
            renderer = std::make_shared<OpenGLRenderer>();
        }
        renderer->init(window, m_settings.screenWidth, m_settings.screenHeight);
    }

    mapWidth = 1920;
    mapHeight = 1064;


#ifndef NDEBUG
    imgui = createImGui(m_settings.rendererType);
    imgui->init(window, renderer);
#endif
    isRunning = true;
}

void Application::run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        update();
        render();
    }
}

void Application::Setup() {
    m_stateMachine = new GameStateMachine();
    m_stateMachine->changeState(new MenuState());
    #ifndef NDEBUG
        auto sceneWidget = std::make_shared<GameSceneWidget>(m_stateMachine->getActiveState()->getRegistry());
        imgui->RegisterWidget("SceneWidget", sceneWidget);
    #endif
}

std::shared_ptr<IRenderer> Application::getRenderer() const {
    return renderer;
}

std::shared_ptr<ImguiSystem> Application::getImguiSystem() const {
    return imgui;
}

int Application::getScreenWidth() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return w;
}

int Application::getScreenHeight() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return h;
}

int Application::getLogicalWidth() {
    return m_settings.screenWidth;
}

int Application::getLogicalHeight() {
    return m_settings.screenHeight;
}

int Application::getMapWidth() {
    return mapWidth;
}

int Application::getMapHeight() {
    return mapHeight;
}

Uint64 Application::getTicks() {
    return SDL_GetTicks();
}

void Application::ProcessInput() {
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent)) {
    #ifndef NDEBUG
        imgui->handleEvent(sdlEvent);
    #endif

        switch (sdlEvent.type) {
            case SDL_EVENT_QUIT:
                quit();
                break;
            case SDL_EVENT_KEY_DOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    quit();
                }
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    isDebug = !isDebug;
                    m_stateMachine->getActiveState()->setDebug(isDebug);
                }
                break;
        }
        m_stateMachine->getActiveState()->handleInput(sdlEvent);
    }
}

void Application::update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();
    m_stateMachine->update(deltaTime);
    for (auto callback : updateCallbacks) {
        callback();
    }
}

void Application::render() {
    renderer->clear();
    m_stateMachine->render(renderer);

#ifndef NDEBUG
    imgui->ShowWidget("MenuBar");
    imgui->render();
#endif
    renderer->present();
}

void Application::cleanup() {
    // AudioManager::instance().cleanup();
#ifndef NDEBUG
    imgui->shutDown();
#endif

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}

void Application::quit() {
    isRunning = false;
}

GameStateMachine* Application::getStateMachine() const {
    return m_stateMachine;
}

void Application::addInitCallback(std::function<void ()> callback) {
    m_initCallback = std::move(callback);
}

void Application::addUpdateCallback(std::function<void ()> callback) {
    updateCallbacks.emplace_back(std::move(callback));
}

}

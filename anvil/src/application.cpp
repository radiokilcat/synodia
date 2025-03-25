#include "application.h"
#include "window.h"
#include "renderer.h"
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
#include <AnvilImgui/ImguiSystem.h>
#include <AnvilImgui/SceneWidget.h>

#include "Logger/Logger.h"

#include "components/TransformComponent.h"
#include "components/RigidBodyComponent.h"
#include "components/SpriteComponent.h"
#include "components/AnimationComponent.h"
#include "components/BoxColliderComponent.h"
#include "components/KeyboardControlledComponent.h"
#include "components/CameraFollowComponent.h"
#include "components/ProjectileEmitterComponent.h"
#include "components/ProjectileComponent.h"
#include "components/HealthComponent.h"
#include "components/TextLabelComponent.h"

#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/RenderColliderSystem.h"
#include "Systems/DamageSystem.h"
#include "Systems/KeyboardControlSystem.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/ProjectileEmitSystem.h"
#include "Systems/ProjectileLifecycleSystem.h"
#include "Systems/RenderHealthBarSystem.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/RenderImGUISystem.h"


namespace anvil {

void GameSettings::validate() {
    assert(screenWidth > 0);
    assert(screenHeight > 0);
    assert(screenScale > 0);
}

static Application* m_instance = nullptr;

Application* Application::Instance() {
    if(m_instance == nullptr)
    {
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

void Application::init(const GameSettings& settings) {
    m_settings = settings;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        std::exit(1);
    }

    int imgFlags = IMG_INIT_PNG;
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
        window = SDL_CreateWindow(m_settings.windowTitle.c_str(), m_settings.screenWidth,
        m_settings.screenHeight, SDL_WINDOW_BORDERLESS);
        SDL_DisplayID display = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);

        if (mode) {
            SDL_Log("Display %" SDL_PRIu32 " mode %d: %dx%d@%gx %gHz\n",
                    display, mode->w, mode->h, mode->pixel_density, mode->refresh_rate);
        }
        renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderLogicalPresentation(renderer,
            m_settings.screenWidth,
            m_settings.screenHeight,
            SDL_LOGICAL_PRESENTATION_STRETCH,
            SDL_SCALEMODE_LINEAR
        );
        if (SDL_SetRenderVSync(renderer, SDL_TRUE) != 0) {
            SDL_Log("Warning: VSync could not be enabled! SDL_Error: %s", SDL_GetError());
        }

    }

    mapWidth = 1920;
    mapHeight = 1064;


#ifndef NDEBUG
    ImguiSystem::Instance()->init(window, renderer);
#endif
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // bg color
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, SDL_ALPHA_OPAQUE);
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
    m_stateMachine->changeState(new PlayState());
    auto sceneWidget = std::make_shared<GameSceneWidget>(m_stateMachine->getActiveState()->getRegistry());
    ImguiSystem::Instance()->RegisterWidget("SceneWidget", sceneWidget);
}

SDL_Renderer* Application::getRenderer() const {
    return renderer;
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

        ImGui_ImplSDL3_ProcessEvent(&sdlEvent);
        ImGuiIO& io = ImGui::GetIO();
        float mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

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
   
}

void Application::render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);

    m_stateMachine->render(renderer);

#ifndef NDEBUG
    ImguiSystem::Instance()->ShowWidget("MenuBar");
    ImguiSystem::Instance()->render();
#endif
    // }


    SDL_RenderPresent(renderer);
}

void Application::cleanup() {
    // InputHandler::instance()->clean();
    // AudioManager::instance().cleanup();
#ifndef NDEBUG
    ImguiSystem::Instance()->shutDown();
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
    m_updateCallback = std::move(callback);
}

}

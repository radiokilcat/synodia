#include "application.h"
#include "window.h"
#include "renderer.h"
#include "inputhandler.h"
// #include "game_state_machine.h"
#include "audio_manager.h"
#include "utils.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>
#include <iostream>
#include <ui/scrollable_text.h>
// #include <AnvilImgui/ImguiSystem.h>

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

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        std::exit(1);
    }
    // AudioManager::instance().initAudio();

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        std::exit(1);
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        Logger::Err("Error initializing SDL TTF.");
        return;
    }

    {
        // window = SDL_CreateWindow(m_settings.windowTitle.c_str(), m_settings.screenWidth,
        // m_settings.screenHeight, SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
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


#ifndef NDEBUG
    // ImguiSystem::Instance()->init(window, renderer);
        // IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)getScreenWidth(), (float)getScreenHeight());
        ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);
#endif

    camera.x = 0;
    camera.y = 0;
    // camera.w = getScreenWidth();
    // camera.h = getScreenHeight();
    camera.w = m_settings.screenWidth;
    camera.h = m_settings.screenHeight;
    Logger::Log("Camera width: " + std::to_string(camera.w) + " height: " + std::to_string(camera.h));

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
    LoadLevel(1);
}

void Application::LoadLevel(int level) {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();
    registry->AddSystem<ProjectileLifecycleSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderHealthBarSystem>();
    registry->AddSystem<RenderGUISystem>();

    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
    assetStore->AddTexture(renderer, "tree-image", "./assets/images/tree.png");
    assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
    assetStore->AddFont("charriot-font-20", "./assets/fonts/charriot.ttf", 20);
    assetStore->AddFont("pico8-font-5", "./assets/fonts/pico8.ttf", 5);
    assetStore->AddFont("pico8-font-10", "./assets/fonts/pico8.ttf", 10);

    int tileSize = 32;
    double tileScale = 2;
    int mapNumCols = 25;
    int mapNumRows = 20;

    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");

    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            float posX = x * (tileScale * tileSize);
            float posY = y * (tileScale * tileSize);


            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    mapWidth = mapNumCols * tileSize * tileScale;
    mapHeight = mapNumRows * tileSize * tileScale;

    Entity chopper = registry->CreateEntity();
    chopper.Tag("player");
    chopper.AddComponent<TransformComponent>(glm::vec2(240.0, 110.0), glm::vec2(1.0, 1.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
    chopper.AddComponent<AnimationComponent>(2, 15, true);
    chopper.AddComponent<BoxColliderComponent>(32, 25, glm::vec2(0, 5));
    chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 10000, 10, true);
    chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -50), glm::vec2(50, 0), glm::vec2(0, 50), glm::vec2(-50, 0));
    chopper.AddComponent<CameraFollowComponent>();
    chopper.AddComponent<HealthComponent>(100);
    
    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2(m_settings.screenWidth - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
    radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
    radar.AddComponent<AnimationComponent>(8, 5, true);
    
    Entity tank = registry->CreateEntity();
    tank.Group("enemies");
    tank.AddComponent<TransformComponent>(glm::vec2(500.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
    tank.AddComponent<BoxColliderComponent>(25, 18, glm::vec2(5, 7));
    tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 3000, 5000, 10, false);
    tank.AddComponent<HealthComponent>(100);

    Entity truck = registry->CreateEntity();
    truck.Group("enemies");
    truck.AddComponent<TransformComponent>(glm::vec2(120.0, 580.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
    truck.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
    truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, -100.0), 2000, 5000, 10, false);
    truck.AddComponent<HealthComponent>(100);

    Entity treeA = registry->CreateEntity();
    treeA.Group("obstacles");
    treeA.AddComponent<TransformComponent>(glm::vec2(600.0, 495.0), glm::vec2(1.0, 1.0), 0.0);
    treeA.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
    treeA.AddComponent<BoxColliderComponent>(16, 32);

    Entity treeB = registry->CreateEntity();
    treeB.Group("obstacles");
    treeB.AddComponent<TransformComponent>(glm::vec2(400.0, 495.0), glm::vec2(1.0, 1.0), 0.0);
    treeB.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
    treeB.AddComponent<BoxColliderComponent>(16, 32);

    Entity label = registry->CreateEntity();
    SDL_Color green = {0, 255, 0};
    label.AddComponent<TextLabelComponent>(glm::vec2(getLogicalWidth() / 2 - 40, 20), "CHOPPER 1.0", "pico8-font-10", green, true);
}

std::shared_ptr<Renderer> Application::getRenderer() const {
    return {};
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
                }
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
                break;
        }
    }
}

void Application::update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();
   
    eventBus->Reset();

    registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

    registry->Update();
    
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
    registry->GetSystem<DamageSystem>().Update();
}

void Application::render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);

    // m_stateMachine->render();

    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetStore, camera);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
#ifndef NDEBUG
        registry->GetSystem<RenderGUISystem>().Update(registry, camera);
    // ImguiSystem::Instance()->ShowWidget("MenuBar");
    // ImguiSystem::Instance()->render();
#endif
    }


    SDL_RenderPresent(renderer);
}

void Application::cleanup() {
    // InputHandler::instance()->clean();
    // AudioManager::instance().cleanup();
#ifndef NDEBUG
    // ImguiSystem::Instance()->shutDown();
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

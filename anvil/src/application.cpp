#include "application.h"
#include "window.h"
#include "renderer.h"
#include "texturemanager.h"
#include "gameobject.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

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

std::filesystem::path getExecutableDir()
{
#ifdef _WIN32
    std::wstring buf;
    buf.resize(MAX_PATH);
    do {
        unsigned int len = GetModuleFileNameW(NULL, &buf[0], static_cast<unsigned int>(buf.size()));
        if (len < buf.size()) {
            buf.resize(len);
            break;
        }

        buf.resize(buf.size() * 2);
    } while (buf.size() < 65536);

    return std::filesystem::path(buf).parent_path();
#else
    if (std::filesystem::exists("/proc/self/exe")) {
        return std::filesystem::read_symlink("/proc/self/exe").parent_path();
    }
    return std::filesystem::path();
#endif
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

Application::Application()
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

    m_textureManager = TextureManager::instance();

    /* For now we load textures in Application::init
     * Probably better to do it in client code */
    if (!m_textureManager->loadTexture((m_resPath / "adventurer.png").string(), "test", m_renderer->getRenderer()))
    {
        printf("could not load texture");
    }
    m_gameObjects.push_back(std::make_unique<Player>(Player(new LoaderParams(100, 100, 50, 37, "test"))));



    SDL_SetRenderDrawBlendMode(m_renderer->getRenderer(), SDL_BLENDMODE_BLEND);

    screenTexture = createBlankTexture(m_renderer->getRenderer(), m_settings.screenWidth, m_settings.screenHeight);

    // bg color
    SDL_SetRenderDrawColor(m_renderer->getRenderer(), 100, 149, 237, SDL_ALPHA_OPAQUE);
}

void Application::main_loop()
{
    while (running)
    {
        std::optional<SDL_Event> event;
        while (event = m_window->pollEvents())
        {
            if (event->type == SDL_EventType::SDL_EVENT_QUIT)
                running = false;
        }
    render();
    }
}

void Application::update()
{

}

void Application::render()
{
    SDL_SetRenderTarget(m_renderer->getRenderer(), screenTexture);

    // render to virtual screen
    SDL_RenderClear(m_renderer->getRenderer());
    SDL_SetRenderTarget(m_renderer->getRenderer(), nullptr);

    // render to screen to window
    SDL_FRect src = {0, 0, static_cast<float>(m_settings.screenWidth), static_cast<float>(m_settings.screenHeight)};
    SDL_FRect dst = {0, 0, static_cast<float>(m_settings.screenWidth) * static_cast<float>(m_settings.screenScale),
                     static_cast<float>(m_settings.screenHeight) * static_cast<float>(m_settings.screenScale)};
    SDL_RenderTexture(m_renderer->getRenderer(), screenTexture, &src, &dst);


    for (const auto& gameObject: m_gameObjects)
    {
        gameObject->draw(m_renderer);
    }


    SDL_RenderPresent(m_renderer->getRenderer());
}

void Application::cleanup()
{

}

}

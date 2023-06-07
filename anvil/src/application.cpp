#include "application.h"
#include "window.h"
#include "renderer.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cassert>


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

std::unique_ptr<Application> Application::create(GameSettings settings)
{
    return std::unique_ptr<Application>(new Application(settings));
}

Application::Application(GameSettings settings)
    : m_settings(settings)
{
}

Application::~Application()
{

}

void Application::run()
{
    init();
    main_loop();
    cleanup();
}

void Application::init()
{
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
    }
    render();
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

    SDL_RenderPresent(m_renderer->getRenderer());
}

void Application::cleanup()
{

}

}

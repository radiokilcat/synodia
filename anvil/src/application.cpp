#include "application.h"

namespace anvil {

std::unique_ptr<Application> Application::create(std::unique_ptr<Window> window)
{
    return std::unique_ptr<Application>(new Application(std::move(window)));
}

Application::Application(std::unique_ptr<Window> window)
    : m_window(std::move(window))
{

}

Application::~Application()
{

}

void Application::handle_loop()
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
}

void Application::update()
{

}

}

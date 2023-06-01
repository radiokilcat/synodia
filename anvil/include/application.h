#pragma once

#include <memory>

#include "window.h"

namespace anvil {

class Application
{
public:
    static std::unique_ptr<Application> create(std::unique_ptr<Window> window);
    Application(std::unique_ptr<Window> window);
    ~Application();

    void init();
    void handle_loop();
    void update();
    void render();
private:
    std::unique_ptr<Window> m_window;
    bool running;
};

}

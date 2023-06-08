#include <iostream>

#include "anvil.h"


int main(int argc, char *argv[])
{
    anvil::GameSettings settings{
        .screenWidth = 800,
        .screenHeight = 600,
        .screenScale = 1,
        .windowTitle = "SDL test app",
    };

    auto app = anvil::Application::create(settings);
    app->run();

    return 0;
}

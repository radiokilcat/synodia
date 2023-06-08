#include <iostream>

#include "anvil.h"


int main(int argc, char *argv[])
{
    anvil::GameSettings settings{
        .screenWidth = 800,
        .screenHeight = 600,
        .screenScale = 3,
        .windowTitle = "SDL test app",
    };
    anvil::Configuration configuration;
    configuration.load("settings.ini");
//    auto app = anvil::Application::create(settings);
//   app->run();

    return 0;
}

#include <iostream>
#include <filesystem>

#include "anvil.h"

#include "gameobjects/player.h"
#include "states/pausestate.h"
#include "states/playstate.h"
#include "states/menustate.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    anvil::Configuration configuration("settings.ini");
    auto settings = configuration.load();
    settings.validate();
    auto app = anvil::Application::Instance();

    app->addInitCallback([]{
        anvil::Application::Instance()->getStateMachine()->changeState(new MenuState);
    });

    app->init(settings);

    app->addUpdateCallback([]{
        if (anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Escape))
        {
            anvil::Application::Instance()->getStateMachine()->pushState(new PauseState);
        }
    });
    app->run();

    return 0;
}

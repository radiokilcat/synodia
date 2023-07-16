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

    std::filesystem::current_path(anvil::getExecutableDir());
    auto resPath = std::filesystem::current_path().parent_path() / "res";

    anvil::Configuration configuration("settings.ini");
    auto settings = configuration.load();
    settings.validate();

    anvil::FontLoader::instance()->loadFont("sample", resPath / "sample.ttf", 16);
    anvil::FontLoader::instance()->setDefaultFont("sample");

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

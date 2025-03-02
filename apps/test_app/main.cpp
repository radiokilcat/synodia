#include <iostream>
#include <filesystem>

#include "anvil.h"

#include "gameobjects/player.h"
#include "gameobjects/gamescene.h"
#include "gameobjects/nonplayable.h"
#include "gameobjects/stationary.h"
#include "gui/menubutton.h"
#include "gui/textbutton.h"
#include "gui/progressBar.h"

#include "states/pausestate.h"
#include "states/playstate.h"
#include "states/menustate.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::filesystem::current_path(anvil::getExecutableDir());
    auto resPath = std::filesystem::current_path() / "assets";

    anvil::Configuration configuration("settings.ini");
    auto settings = configuration.load();
    settings.validate();

    anvil::FontLoader::instance()->loadFont("sample", resPath / "fonts" / "DungeonFont.ttf", 14);
    anvil::FontLoader::instance()->setDefaultFont("sample");

    auto app = anvil::Application::Instance();

    anvil::TileMap::registerWithFactory();
    anvil::TextLabel::registerWithFactory();
    anvil::VisualNode::registerWithFactory();
    GameScene::registerWithFactory();
    Player::registerWithFactory();
    NonPlayable::registerWithFactory();
    MenuButton::registerWithFactory();
    TextButton::registerWithFactory();
    GUIProgressBar::registerWithFactory();
    Stationary::registerWithFactory();

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

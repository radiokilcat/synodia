#include <iostream>
#include <filesystem>

#include "anvil.h"
#include "utils.h"

#include "player.h"

int main(int argc, char *argv[])
{
    anvil::Configuration configuration("settings.ini");
    auto settings = configuration.load();
    settings.validate();

    auto app = anvil::Application::Instance();
    app->init(settings);

    std::filesystem::current_path(anvil::getExecutableDir());
    auto resPath = std::filesystem::current_path().parent_path() / "res";

    // ToDo: Texture loading and Gameobject should be deserialised from json
    anvil::TextureManager::instance()->loadTexture((resPath / "adventurer.png").string(), "test", app->getRenderer()->getRenderer());
    app->addGameObject(std::make_unique<Player>(Player(new anvil::LoaderParams(100, 100, 50, 37, "test"))));
    app->run();

    return 0;
}

#include <filesystem>

#include "anvil.h"
#include "playstate.h"
#include "../gameobjects/gamescene.h"
#include "../gameobjects/player.h"


PlayState::PlayState()
{

}

bool PlayState::onEnter()
{
    std::filesystem::current_path(anvil::getExecutableDir());
    auto resPath = std::filesystem::current_path().parent_path() / "res";
    auto app = anvil::Application::Instance();

//     ToDo: Texture loading and Gameobject should be deserialised from json
    anvil::TextureManager::instance()->loadTexture((resPath / "adventurer.png").string(), "test", app->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture((resPath / "tiles" / "water.png").string(), "water", app->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture((resPath / "tiles" / "sand.png").string(), "sand", app->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture((resPath / "tiles" / "grass.png").string(), "grass", app->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture((resPath / "tiles" / "tile-11.png").string(), "eleven", app->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture((resPath / "tiles" / "tile-9.png").string(), "hill", app->getRenderer()->getRenderer());

    m_scene = std::make_unique<GameScene>();
    m_scene->addGameObject(std::make_unique<anvil::TileMap>(new anvil::LoaderParams(100, 100, 50, 37, "water")));
    m_scene->addGameObject(std::make_unique<anvil::GameObject>(new anvil::LoaderParams(100, 100, 50, 37, "test")));

    std::cout << "Enter Play state" << std::endl;
    return true;
}

bool PlayState::onExit()
{
    std::cout << "Enter Exit state" << std::endl;
    return true;
}

void PlayState::update()
{
    if (m_scene)
    {
        m_scene->update();
    }
}

void PlayState::render()
{
    m_scene->draw(anvil::Application::Instance()->getRenderer());
}

std::string PlayState::getID()
{
    return m_id;
}

#include <filesystem>

#include "anvil.h"
#include "playstate.h"
#include "../gameobjects/player.h"


namespace anvil {

PlayState::PlayState()
{

}

bool PlayState::onEnter()
{
//    std::filesystem::current_path(anvil::getExecutableDir());
//    auto resPath = std::filesystem::current_path().parent_path() / "res";
//    auto app = anvil::Application::Instance();


    // ToDo: Texture loading and Gameobject should be deserialised from json
//    TextureManager::instance()->loadTexture((resPath / "adventurer.png").string(), "test", app->getRenderer()->getRenderer());
//    TextureManager::instance()->loadTexture((resPath / "tiles" / "water.png").string(), "water", app->getRenderer()->getRenderer());
//    TextureManager::instance()->loadTexture((resPath / "tiles" / "sand.png").string(), "sand", app->getRenderer()->getRenderer());
//    TextureManager::instance()->loadTexture((resPath / "tiles" / "grass.png").string(), "grass", app->getRenderer()->getRenderer());
//    TextureManager::instance()->loadTexture((resPath / "tiles" / "tile-11.png").string(), "eleven", app->getRenderer()->getRenderer());
//    TextureManager::instance()->loadTexture((resPath / "tiles" / "tile-9.png").string(), "hill", app->getRenderer()->getRenderer());

//    m_gameObjects.push_back(new TileMap(new LoaderParams(100, 100, 50, 37, "water")));
//    m_gameObjects.push_back(new GameObject(new LoaderParams(100, 100, 50, 37, "test")));

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
    for (auto it: m_gameObjects)
    {
        it->update();
    }
}

void PlayState::render()
{
    for (auto it: m_gameObjects)
    {
        it->draw(Application::Instance()->getRenderer());
    }
}

std::string PlayState::getID()
{
    return m_id;
}


}

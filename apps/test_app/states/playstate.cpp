#include <filesystem>

#include "anvil.h"
#include "playstate.h"
#include "../gameobjects/gamescene.h"
#include "../gameobjects/player.h"
#include "../gameobjects/speech.h"


PlayState::PlayState()
{

}

bool PlayState::onEnter()
{
    m_textureIds = anvil::StateLoader::loadTextures(m_id);

    if (m_scene == nullptr)
    {
        m_scene = new GameScene();

        auto map = anvil::GameObjectFactory::instance().createGameObject("TileMap");
        auto player = anvil::GameObjectFactory::instance().createGameObject("Player");

        map->load(new anvil::LoaderParams(100, 100, 50, 37, "water"));
        player->load(new anvil::LoaderParams(100, 100, 50, 37, "test"));

        m_scene->addGameObject(std::move(map));
        m_scene->addGameObject(std::move(player));
    }
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

void PlayState::to_json(nlohmann::json& j) 
{
    m_scene->to_json(j["m_scene"]);
}

void PlayState::from_json(nlohmann::json& j)
{
    m_scene = new GameScene;
    m_scene->from_json(j["m_scene"]);
}

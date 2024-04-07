#include <filesystem>

#include "anvil.h"
#include "playstate.h"


PlayState::PlayState()
{

}

bool PlayState::onEnter()
{
    m_textureIds = anvil::StateLoader::loadTextures(m_id);
    anvil::StateLoader::loadAudio(m_id);
    m_scene = anvil::StateLoader::loadGameObjects(m_id);
    auto rootScene = dynamic_cast<anvil::GameObject*>(m_scene.get());
    anvil::ImguiSystem::Instance()->setRootNode(rootScene);

//    m_scene->setTileMap(std::make_unique<anvil::TileMap>(new anvil::GameObjectData(100, 100, 50, 37, "water")));
//    m_scene->setPlayer(std::make_unique<Player>(new anvil::GameObjectData(375, 100, 64, 64, "player_1")));
        
    std::cout << "Enter Play state" << std::endl;
    return true;
}

bool PlayState::onExit()
{
    m_scene->clean();
    for (auto it: m_textureIds)
    {
        anvil::TextureManager::instance()->clearFromTextureMap(it);
    }
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
    m_scene->to_json(j["GameScene"]);
}

void PlayState::from_json(const nlohmann::json& j)
{
    if (!m_scene)
    {
        m_scene = anvil::StateLoader::loadGameObjects(m_id);
    }
    m_scene->from_json(j["GameScene"]);
}

//std::vector<anvil::IGameObject> PlayState::objects() {
//    std::vector<anvil::IGameObject> result;
//    result.push_back(m_scene);
//    return std::vector<anvil::IGameObject>();
//}

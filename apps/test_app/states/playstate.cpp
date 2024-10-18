#include <filesystem>

#include "anvil.h"
#include "playstate.h"
#include "../gameobjects/gamescene.h"

#include "texturemanager.h"
#include "components/MovementIsoComponent.h"


PlayState::PlayState() {

}

bool PlayState::onEnter() {
    anvil::StateLoader::loadAudio(m_id);
    m_textureIds = anvil::StateLoader::loadTextures(m_id);
    m_scene = anvil::StateLoader::loadGameObjects(m_id);
    auto rootScene = std::dynamic_pointer_cast<GameScene>(m_scene);
    anvil::ImguiSystem::Instance()->setScene(rootScene);
    rootScene->baseInit();
    std::cout << "Enter Play state" << std::endl;
    return true;
}

bool PlayState::onExit() {
    m_scene->clean();
    for (auto it: m_textureIds) {
        anvil::TextureManager::instance()->clearFromTextureMap(it);
    }
    std::cout << "Enter Exit state" << std::endl;
    return true;
}

void PlayState::update(Uint64 deltaTime) {
    if (m_scene) {
        m_scene->update(deltaTime);
    }
}

void PlayState::render() {
    m_scene->draw(anvil::Application::Instance()->getRenderer());
}

std::string PlayState::getID() {
    return m_id;
}

void PlayState::to_json(nlohmann::json& j) {
    m_scene->to_json(j["GameScene"]);
}

void PlayState::from_json(const nlohmann::json& j) {
    if (!m_scene) {
        m_scene = anvil::StateLoader::loadGameObjects(m_id);
    }
    m_scene->from_json(j["GameScene"]);
}
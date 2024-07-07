#include <filesystem>

#include "anvil.h"
#include "mainstate.h"

#include "texturemanager.h"
#include "components/MovementIsoComponent.h"


MainState::MainState() {

}

bool MainState::onEnter() {
    // m_textureIds = anvil::StateLoader::loadTextures(m_id);
    // m_scene = anvil::StateLoader::loadGameObjects(m_id);
    // auto rootScene = std::dynamic_pointer_cast<anvil::GameObject>(m_scene);
    // anvil::ImguiSystem::Instance()->setRootNode(rootScene);
    // rootScene->baseInit();
    std::cout << "Enter Main state" << std::endl;
    return true;
}

bool MainState::onExit() {
    // m_scene->clean();
    // for (auto it: m_textureIds) {
    //     anvil::TextureManager::instance()->clearFromTextureMap(it);
    // }
    // std::cout << "Enter Exit state" << std::endl;
    return true;
}

void MainState::update(Uint64 deltaTime) {
    // if (m_scene) {
    //     m_scene->update(deltaTime);
    // }
}

void MainState::render() {
    // m_scene->draw(anvil::Application::Instance()->getRenderer());
}

std::string MainState::getID() {
    return m_id;
}

void MainState::to_json(nlohmann::json& j) {
    // m_scene->to_json(j["GameScene"]);
}

void MainState::from_json(const nlohmann::json& j) {
    // if (!m_scene) {
    //     m_scene = anvil::StateLoader::loadGameObjects(m_id);
    // }
    // m_scene->from_json(j["GameScene"]);
}
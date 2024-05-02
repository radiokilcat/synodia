#include "anvil.h"
#include "editstate.h"

#include "texturemanager.h"

std::string EditState::getID() {
    return m_id;
}

bool EditState::onEnter() {
    m_textureIds = anvil::StateLoader::loadTextures(m_id);
    m_scene = anvil::StateLoader::loadGameObjects(m_id);
    auto rootScene = std::dynamic_pointer_cast<anvil::GameObject>(m_scene);
    rootScene->baseInit();
    anvil::ImguiSystem::Instance()->setRootNode(rootScene);
    std::cout << "Enter Edit state" << std::endl;
    return true;
}

void EditState::render() {
    m_scene->draw(anvil::Application::Instance()->getRenderer());
}

bool EditState::onExit() {
    for (auto it: m_gameObjects) {
        it->clean();
    }
    m_gameObjects.clear();
    for (auto it: m_textureIds) {
        anvil::TextureManager::instance()->clearFromTextureMap(it);
    }
    std::cout << "Exit Edit state" << std::endl;
    return true;
}

void EditState::update(Uint64 deltaTime) {
}


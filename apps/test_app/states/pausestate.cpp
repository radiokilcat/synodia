#include "pausestate.h"
#include "playstate.h"
#include "../gui/textbutton.h"
#include "../gui/menubutton.h"

#include <iostream>

#include "components/TextComponent.h"

PauseState::PauseState()
{

}

bool PauseState::onEnter()
{
    m_textureIds = anvil::StateLoader::loadTextures(m_id);
    anvil::StateLoader::loadAudio(m_id);
    anvil::AudioManager::instance().playMusic("theme");
    
    const float BUTTON_WIDTH = 400.f;
    const auto SCREEN_WIDTH = static_cast<float>(anvil::Application::Instance()->getScreenWidth());
    const float HALF_SCREEN_X = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;

    
    m_gameMenu = anvil::StateLoader::loadGameObjects(m_id);
    auto rootScene = std::dynamic_pointer_cast<anvil::GameObject>(m_gameMenu);
    anvil::ImguiSystem::Instance()->setRootNode(rootScene);
    rootScene->init();
    if (auto title = rootScene->findChild("title")) {
        auto transform = title->getComponent<anvil::Transform2DComponent>();
        transform->setPosition(anvil::Vector2D{SCREEN_WIDTH / 2 - 250, 20});
    };
    if (auto button = rootScene->findChild("button1")) {
        auto button1 = std::dynamic_pointer_cast<MenuButton>(button);
        auto transform = button1->getComponent<anvil::Transform2DComponent>();
        transform->setPosition(anvil::Vector2D{HALF_SCREEN_X, 300});
        button1->setCallback([]() {
            std::cout << "Play button clicked" << std::endl;
            anvil::Application::Instance()->getStateMachine()->popState();
        });
    };
    if (auto button = rootScene->findChild("button2")) {
        auto button3 = std::dynamic_pointer_cast<MenuButton>(button);
        auto transform = button3->getComponent<anvil::Transform2DComponent>();
        transform->setPosition(anvil::Vector2D{HALF_SCREEN_X, 450});
        button3->setCallback([]() {
            std::cout << "Load button clicked" << std::endl;
            auto serializer = anvil::JsonSerializer("output.txt");
            auto playstate = anvil::Application::Instance()->getStateMachine()->findState("play");
            serializer.serialize((PlayState *)playstate);
        });
    };
    if (auto button = rootScene->findChild("button3")) {
        auto button2 = std::dynamic_pointer_cast<MenuButton>(button);
        auto transform = button2->getComponent<anvil::Transform2DComponent>();
        transform->setPosition(anvil::Vector2D{HALF_SCREEN_X, 600});
        button2->setCallback([]() {
            std::cout << "Exit button clicked" << std::endl;
            anvil::Application::Instance()->quit();
        });
        
    };
    std::cout << "Enter Pause state" << std::endl;
    return true;
}

bool PauseState::onExit()
{
    for (auto it: m_gameObjects)
    {
        it->clean();
    }
    m_gameObjects.clear();
    for (const auto& it: m_textureIds)
    {
        anvil::TextureManager::instance()->clearFromTextureMap(it);
    }
    std::cout << "Exit Pause state" << std::endl;
    return true;
}

void PauseState::update(Uint64 deltaTime)
{
    m_gameMenu->update(deltaTime);
}

void PauseState::render()
{
    auto renderer = anvil::Application::Instance()->getRenderer();
    anvil::TextureManager::instance()->draw("background", 0, 0,
        static_cast<float>(anvil::Application::Instance()->getScreenWidth()),
        static_cast<float>(anvil::Application::Instance()->getScreenHeight()),
        renderer->getRenderer());
    
    m_gameMenu->draw(anvil::Application::Instance()->getRenderer());
}

std::string PauseState::getID()
{
    return m_id;
}


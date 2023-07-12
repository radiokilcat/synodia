#include "pausestate.h"
#include "playstate.h"

#include "../gameobjects/menubutton.h"

#include <iostream>

PauseState::PauseState()
{

}

bool PauseState::onEnter()
{
    anvil::TextureManager::instance()->loadTexture("res/button.png", "playbutton", anvil::Application::Instance()->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture("res/exit.png", "exitbutton", anvil::Application::Instance()->getRenderer()->getRenderer());

    anvil::GameObject* button1 = new MenuButton(new anvil::LoaderParams(100, 100, 400, 100, "playbutton"), []() {
        std::cout << "Play button clicked" << std::endl;
        anvil::Application::Instance()->getStateMachine()->popState();
    });

    anvil::GameObject* button2 = new MenuButton(new anvil::LoaderParams(100, 300, 400, 100, "exitbutton"), []() {
        std::cout << "Exit button clicked" << std::endl;
        anvil::Application::Instance()->quit();
    });

    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);

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
    anvil::TextureManager::instance()->clearFromTextureMap("playbutton");
    anvil::TextureManager::instance()->clearFromTextureMap("exitbutton");
    std::cout << "Exit Pause state" << std::endl;
    return true;
}

void PauseState::update()
{
    for (auto it: m_gameObjects)
    {
        it->update();
    }
}

void PauseState::render()
{
    for (auto it: m_gameObjects)
    {
        it->draw(anvil::Application::Instance()->getRenderer());
    }
}

std::string PauseState::getID()
{
    return m_id;
}


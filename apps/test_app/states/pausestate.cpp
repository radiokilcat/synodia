#include "anvil.h"

#include "pausestate.h"
#include "playstate.h"

#include "../gameobjects/menubutton.h"

#include <iostream>

namespace anvil {

PauseState::PauseState()
{

}

bool PauseState::onEnter()
{
    TextureManager::instance()->loadTexture("res/button.png", "playbutton", Application::Instance()->getRenderer()->getRenderer());
    TextureManager::instance()->loadTexture("res/exit.png", "exitbutton", Application::Instance()->getRenderer()->getRenderer());

    GameObject* button1 = new MenuButton(new LoaderParams(100, 100, 400, 100, "playbutton"), []() {
        std::cout << "Play button clicked" << std::endl;
        Application::Instance()->getStateMachine()->popState();
    });

    GameObject* button2 = new MenuButton(new LoaderParams(100, 300, 400, 100, "exitbutton"), []() {
        std::cout << "Exit button clicked" << std::endl;
        Application::Instance()->quit();
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
    TextureManager::instance()->clearFromTextureMap("playbutton");
    TextureManager::instance()->clearFromTextureMap("exitbutton");
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
        it->draw(Application::Instance()->getRenderer());
    }
}

std::string PauseState::getID()
{
    return m_id;
}

}

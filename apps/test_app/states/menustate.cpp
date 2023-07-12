#include "anvil.h"

#include "menustate.h"
#include "playstate.h"

#include "../gameobjects/menubutton.h"

namespace anvil {

std::string MenuState::getID()
{
    return m_id;
}

bool MenuState::onEnter()
{
    TextureManager::instance()->loadTexture("res/button.png", "playbutton", Application::Instance()->getRenderer()->getRenderer());
    TextureManager::instance()->loadTexture("res/exit.png", "exitbutton", Application::Instance()->getRenderer()->getRenderer());

    GameObject* button1 = new MenuButton(new LoaderParams(100, 100, 400, 100, "playbutton"), []() {
        std::cout << "Play button clicked" << std::endl;
        Application::Instance()->getStateMachine()->changeState(new PlayState);
    });

    GameObject* button2 = new MenuButton(new LoaderParams(100, 300, 400, 100, "exitbutton"), []() {
        std::cout << "Exit button clicked" << std::endl;
        Application::Instance()->quit();
    });

    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);

    std::cout << "Enter Menu state" << std::endl;
    return true;
}

void MenuState::render()
{
    for (auto it: m_gameObjects)
    {
        it->draw(Application::Instance()->getRenderer());
    }
}

bool MenuState::onExit()
{
    for (auto it: m_gameObjects)
    {
        it->clean();
    }
    m_gameObjects.clear();
    TextureManager::instance()->clearFromTextureMap("playbutton");
    TextureManager::instance()->clearFromTextureMap("exitbutton");
    std::cout << "Exit Menu state" << std::endl;
    return true;
}

void MenuState::update()
{
    for (auto it: m_gameObjects)
    {
        it->update();
    }
}

}


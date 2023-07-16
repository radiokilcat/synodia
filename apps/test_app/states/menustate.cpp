#include "anvil.h"

#include "menustate.h"
#include "playstate.h"

#include "../gui/menubutton.h"
#include "../gui/textbutton.h"

std::string MenuState::getID()
{
    return m_id;
}

bool MenuState::onEnter()
{
    anvil::TextureManager::instance()->loadTexture("res/button.png", "playbutton", anvil::Application::Instance()->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture("res/exit.png", "exitbutton", anvil::Application::Instance()->getRenderer()->getRenderer());

    anvil::GameObject* button1 = new MenuButton(new anvil::LoaderParams(100, 100, 400, 100, "playbutton"), []() {
        std::cout << "Play button clicked" << std::endl;
        anvil::Application::Instance()->getStateMachine()->changeState(new PlayState);
    });

    anvil::GameObject* button2 = new MenuButton(new anvil::LoaderParams(100, 300, 400, 100, "exitbutton"), []() {
        std::cout << "Exit button clicked" << std::endl;
        anvil::Application::Instance()->quit();
    });

    anvil::GameObject* button3 = new TextButton(new anvil::LoaderParams(100, 330, 400, 100, "debug"), "test_text", []() {
        std::cout << "Exit button clicked" << std::endl;
        anvil::Application::Instance()->quit();
    });

    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);
    m_gameObjects.push_back(button3);

    std::cout << "Enter Menu state" << std::endl;
    return true;
}

void MenuState::render()
{
    for (auto it: m_gameObjects)
    {
        it->draw(anvil::Application::Instance()->getRenderer());
    }
}

bool MenuState::onExit()
{
    for (auto it: m_gameObjects)
    {
        it->clean();
    }
    m_gameObjects.clear();
    anvil::TextureManager::instance()->clearFromTextureMap("playbutton");
    anvil::TextureManager::instance()->clearFromTextureMap("exitbutton");
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


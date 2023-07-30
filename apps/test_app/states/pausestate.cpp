#include "pausestate.h"
#include "playstate.h"
#include "../gui/textbutton.h"
#include "../gui/menubutton.h"

#include <iostream>

PauseState::PauseState()
{

}

bool PauseState::onEnter()
{
    auto resPath = std::filesystem::current_path().parent_path() / "res";

    anvil::TextureManager::instance()->loadTexture((resPath / "button.png").string(), "playbutton", anvil::Application::Instance()->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture((resPath / "exit.png").string(), "exitbutton", anvil::Application::Instance()->getRenderer()->getRenderer());
    anvil::TextureManager::instance()->loadTexture((resPath / "empty_button.png").string(), "empty_button", anvil::Application::Instance()->getRenderer()->getRenderer());

    MenuButton* button1 = new MenuButton();
    button1->setCallback([]() {
        std::cout << "Play button clicked" << std::endl;
        anvil::Application::Instance()->getStateMachine()->popState();
    });

    MenuButton* button2 = new MenuButton();
    button2->setCallback([]() {
        std::cout << "Exit button clicked" << std::endl;
        anvil::Application::Instance()->quit();
    });
    button2->load(new anvil::LoaderParams(100, 300, 400, 100, "exitbutton"));

    TextButton* button3 = new TextButton();
    button3->setCallback([]() {
        std::cout << "Save button clicked" << std::endl;
        auto serializer = anvil::JsonSerializer("output.txt");
        auto playstate = anvil::Application::Instance()->getStateMachine()->findState("play");
        serializer.serialize((PlayState *)playstate);
    });
    button3->setText("Save Game");
    button3->load(new anvil::LoaderParams(100, 460, 400, 100, "empty_button"));

    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);
    m_gameObjects.push_back(button3);


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


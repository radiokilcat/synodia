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
    m_textureIds = anvil::StateLoader::loadTextures(m_id);
    const float BUTTON_WIDTH = 400.f;
    const auto SCREEN_WIDTH = static_cast<float>(anvil::Application::Instance()->getScreenWidth());
    const float HALF_SCREEN_X = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;

    anvil::Color textColor = { 255, 255, 255 };
    auto title = new anvil::TextLabel("Paused", textColor);
    title->load(new anvil::LoaderParams(SCREEN_WIDTH / 2 - 150, 20, 300, 80, "Paused"));

    auto button1 = new TextButton();
    button1->setText("Play Game");
    button1->setCallback([]() {
        std::cout << "Play button clicked" << std::endl;
        anvil::Application::Instance()->getStateMachine()->popState();
    });
    button1->load(new anvil::LoaderParams(HALF_SCREEN_X, 150, 400, 100, "empty_button"));

    auto button2 = new TextButton();
    button2->setText("Exit Game");
    button2->setCallback([]() {
        std::cout << "Exit button clicked" << std::endl;
        anvil::Application::Instance()->quit();
    });
    button2->load(new anvil::LoaderParams(HALF_SCREEN_X, 300, 400, 100, "empty_button"));

    auto button3 = new TextButton();
    button3->setCallback([]() {
        std::cout << "Save button clicked" << std::endl;
        auto serializer = anvil::JsonSerializer("output.txt");
        auto playstate = anvil::Application::Instance()->getStateMachine()->findState("play");
        serializer.serialize((PlayState *)playstate);
    });
    button3->setText("Save Game");
    button3->load(new anvil::LoaderParams(HALF_SCREEN_X, 460, 400, 100, "empty_button"));
    m_gameObjects.push_back(title);
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
    for (const auto& it: m_textureIds)
    {
        anvil::TextureManager::instance()->clearFromTextureMap(it);
    }
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
    auto renderer = anvil::Application::Instance()->getRenderer();
    anvil::TextureManager::instance()->draw("background",
        0,
        0,
        static_cast<float>(anvil::Application::Instance()->getScreenWidth()),
        static_cast<float>(anvil::Application::Instance()->getScreenHeight()),
        renderer->getRenderer());
    for (auto it: m_gameObjects)
    {
        it->draw(anvil::Application::Instance()->getRenderer());
    }
}

std::string PauseState::getID()
{
    return m_id;
}


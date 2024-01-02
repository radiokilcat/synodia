#include "anvil.h"

#include "losestate.h"
#include "menustate.h"
#include "playstate.h"

#include "../gui/menubutton.h"
#include "../gui/textbutton.h"
#include "../gameobjects/nonplayable.h"
#include "../gameobjects/gamescene.h"

std::string LoseState::getID()
{
    return m_id;
}

bool LoseState::onEnter()
{
    m_textureIds = anvil::StateLoader::loadTextures(m_id);

    anvil::Color textColor = { 255, 255, 255 };
    const float BUTTON_WIDTH = 400.f;
    const auto SCREEN_WIDTH = static_cast<float>(anvil::Application::Instance()->getScreenWidth());
    const auto SCREEN_HEIGHT = static_cast<float>(anvil::Application::Instance()->getScreenHeight());
    const float HALF_SCREEN_X = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;

    auto title = new anvil::TextLabel("YOU DIED!!!", textColor);
    title->load(new anvil::LoaderParams(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 , 600, 80, "title"));

    auto button1 = new TextButton();
    button1->setText("Play again");
    button1->load(new anvil::LoaderParams(HALF_SCREEN_X, SCREEN_HEIGHT / 4 + 140, 400, 100, "empty_button"));
    button1->setCallback([]() {
        std::cout << "Play button clicked" << std::endl;
        anvil::Application::Instance()->getStateMachine()->changeState(new PlayState);
    });

    auto button2 = new TextButton();
    button2->setText("Exit Game");
    button2->load(new anvil::LoaderParams(HALF_SCREEN_X, SCREEN_HEIGHT / 4 + 280, 400, 100, "empty_button"));
    button2->setCallback([]() {
        std::cout << "Exit button clicked" << std::endl;
        anvil::Application::Instance()->quit();
    });

    m_gameObjects.push_back(title);
    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);

    std::cout << "Enter Lose state" << std::endl;
    return true;
}

void LoseState::render()
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
        it->draw(renderer);
    }
}

bool LoseState::onExit()
{
    for (auto it: m_gameObjects)
    {
        it->clean();
    }
    m_gameObjects.clear();
    for (auto it: m_textureIds)
    {
        anvil::TextureManager::instance()->clearFromTextureMap(it);
    }
    std::cout << "Exit Menu state" << std::endl;
    return true;
}

void LoseState::update()
{
    for (auto it: m_gameObjects)
    {
        it->update();
    }
}


#include "anvil.h"

#include "menustate.h"
#include "playstate.h"

#include "../gui/menubutton.h"
#include "../gui/textbutton.h"
#include "../gameobjects/nonplayable.h"
#include "../gameobjects/gamescene.h"

std::string MenuState::getID()
{
    return m_id;
}

bool MenuState::onEnter()
{
    m_textureIds = anvil::StateLoader::loadTextures(m_id);
    anvil::StateLoader::loadAudio(m_id);
    anvil::AudioManager::instance().playMusic("theme");


    anvil::Color textColor = { 255, 255, 255 };
    const float BUTTON_WIDTH = 400.f;
    const auto SCREEN_WIDTH = static_cast<float>(anvil::Application::Instance()->getScreenWidth());
    const float HALF_SCREEN_X = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;

    auto title = new anvil::TextLabel("Stories of Anvil", textColor);
    title->load(new anvil::LoaderParams(SCREEN_WIDTH / 2 - 250, 20, 500, 80, "title"));

    auto button1 = new TextButton();
    button1->setText("Play Game");
    button1->load(new anvil::LoaderParams(HALF_SCREEN_X, 150, 400, 100, "empty_button"));
    button1->setCallback([]() {
        std::cout << "Play button clicked" << std::endl;
        anvil::Application::Instance()->getStateMachine()->changeState(new PlayState);
    });

    auto button2 = new TextButton();
    button2->setText("Exit Game");
    button2->load(new anvil::LoaderParams(HALF_SCREEN_X, 300, 400, 100, "empty_button"));
    button2->setCallback([]() {
        std::cout << "Exit button clicked" << std::endl;
        anvil::Application::Instance()->quit();
    });

    auto button3 = new TextButton();
    button3->setText("Load Game");
    button3->load(new anvil::LoaderParams(HALF_SCREEN_X, 450, 400, 100, "empty_button"));
    button3->setCallback([]() {
        std::cout << "Load button clicked" << std::endl;
        auto serializer = anvil::JsonSerializer("output.txt");
        auto playLoad = new PlayState();
        serializer.deserialize(playLoad);
        anvil::Application::Instance()->getStateMachine()->changeState(playLoad);
    });
    
    m_gameObjects.push_back(title);
    m_gameObjects.push_back(button1);
    m_gameObjects.push_back(button2);
    m_gameObjects.push_back(button3);

    std::cout << "Enter Menu state" << std::endl;
    return true;
}

void MenuState::render()
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

bool MenuState::onExit()
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


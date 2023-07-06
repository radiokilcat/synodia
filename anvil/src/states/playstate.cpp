#include "playstate.h"


namespace anvil {

PlayState::PlayState()
{

}

bool PlayState::onEnter()
{
    std::cout << "Enter Play state" << std::endl;
    return true;
}

bool PlayState::onExit()
{
    std::cout << "Enter Exit state" << std::endl;
    return true;
}

void PlayState::update()
{
    for (auto it: m_gameObjects)
    {
        it->update();
    }
}

void PlayState::render()
{
    for (auto it: m_gameObjects)
    {
        it->draw(Application::Instance()->getRenderer());
    }
}

std::string PlayState::getID()
{
    return m_id;
}


}

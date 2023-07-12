#pragma once

#include "gamestate.h"
#include <vector>


namespace anvil {

class GameObject;

class PauseState : public GameState
{
public:
    PauseState();
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

private:

    std::vector<GameObject*> m_gameObjects;
    std::string m_id = "PAUSE";
};


}

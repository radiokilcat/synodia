#pragma once

#include <string>
#include <vector>

#include "gamestate.h"
#include "gameobject.h"

namespace anvil {

class MenuState : public GameState
{
public:
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

private:
    const std::string m_id = "MENU";
    std::vector<GameObject*> m_gameObjects;
};

}

#pragma once

#include <string>
#include <vector>

#include "gamestate.h"
#include "gameobject.h"

namespace anvil {

class MenuState : public GameState
{
    friend class MenuStateSerializer;
public:
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

protected:
    const std::string m_id = "MENU";
    std::vector<GameObject*> m_gameObjects;
};

}

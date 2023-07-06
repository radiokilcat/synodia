#pragma once

#include "gamestate.h"

namespace anvil {


class PlayState : public GameState
{
public:
    PlayState();
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

private:

    std::vector<GameObject*> m_gameObjects;
    const std::string m_id = "play";
};


}

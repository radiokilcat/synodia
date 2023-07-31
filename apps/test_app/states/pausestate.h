#pragma once

#include "anvil.h"
#include <vector>


class PauseState : public anvil::GameState
{
public:
    PauseState();
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

private:

    std::vector<anvil::GameObject*> m_gameObjects;
    std::string m_id = "PAUSE";
    std::vector<std::string> m_textureIds;
};

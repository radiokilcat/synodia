#pragma once

#include <string>
#include <vector>

#include "anvil.h"

class WinState : public anvil::GameState
{
public:
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

private:
    const std::string m_id = "WIN";
    std::vector<anvil::BaseGameObject*> m_gameObjects;
    std::vector<std::string> m_textureIds;
};

#pragma once

#include <string>
#include <vector>

#include "anvil.h"

class WinState : public anvil::GameState
{
public:
    bool onEnter() override;
    bool onExit() override;

    void update(Uint64 deltaTime) override;
    void render() override;

    std::string getID() override;

private:
    const std::string m_id = "WIN";
    std::vector<std::string> m_textureIds;
    std::shared_ptr<anvil::IGameObject> m_gameMenu;
};

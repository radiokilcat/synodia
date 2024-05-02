#pragma once

#include "anvil.h"
#include <vector>


class PauseState : public anvil::GameState
{
public:
    PauseState();
    bool onEnter() override;
    bool onExit() override;

    void update(Uint64 deltaTime) override;
    void render() override;

    std::string getID() override;

private:
    std::shared_ptr<anvil::IGameObject> m_gameMenu;
    std::vector<anvil::GameObject*> m_gameObjects;
    std::string m_id = "PAUSE";
    std::vector<std::string> m_textureIds;
};

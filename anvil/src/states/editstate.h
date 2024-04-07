#pragma once

#include <string>
#include <vector>

#include "anvil.h"

class EditState : public anvil::GameState
{
public:
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

private:
    std::unique_ptr<anvil::IGameObject> m_scene = nullptr;
    const std::string m_id = "EDIT";
    std::vector<anvil::IGameObject*> m_gameObjects;
    std::vector<std::string> m_textureIds;
};

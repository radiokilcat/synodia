#pragma once

#include <string>
#include <vector>
#include "gamestate.h"
#include "stateLoader.hpp"
#include "../ECS/ECS.h"
#include "../Render/IRenderer.hpp"

namespace anvil {

class EditState : public GameState
{
public:
    EditState() {}
    bool onEnter() override;
    bool onExit() override;

    void update(double deltaTime) override;
    void render(std::shared_ptr<IRenderer> renderer) override;
    void handleInput(SDL_Event& event) override;

    std::string getID() override { return m_id; };
    void setDebug(bool debug) override { isDebug = debug; }

private:
    std::unique_ptr<Registry> registry;
    std::unique_ptr<StateLoader> stateLoader;
    const std::string m_id = "EDIT";
    std::vector<std::string> m_textureIds;
    bool isDebug = false;
};

}
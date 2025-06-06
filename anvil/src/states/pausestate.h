#pragma once

#include <string>

#include "gamestate.h"
#include "stateLoader.hpp"
#include "../ECS/ECS.h"
#include "../Render/IRenderer.hpp"

namespace anvil {

class PauseState : public anvil::GameState
{
public:
    PauseState() {}
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
    std::string m_id = "PAUSE";
    std::vector<std::string> m_textureIds;
    bool isDebug = false;
};

}
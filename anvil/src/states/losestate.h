#pragma once

#include <string>
#include <vector>
#include "gamestate.h"
#include "stateLoader.hpp"
#include "../ECS/ECS.h"

namespace anvil {

class LoseState : public GameState {
public:
    LoseState() { }
    bool onEnter() override;
    bool onExit() override;

    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    void handleInput(SDL_Event& event) override;

    void setDebug(bool debug) override { isDebug = debug; }

    std::string getID() override { return m_id; };

private:
    std::unique_ptr<Registry> registry;
    std::unique_ptr<StateLoader> stateLoader;
    const std::string m_id = "LOSE";
    std::vector<std::string> m_textureIds;
    bool isDebug = false;
};

}
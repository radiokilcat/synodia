#pragma once

#include <string>
#include <vector>

#include "gamestate.h"
#include "../ECS/ECS.h"
#include "stateLoader.hpp"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

namespace anvil {

class MenuState : public GameState
{
public:
    MenuState();
    bool onEnter() override;
    bool onExit() override;

    void update(double deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    void handleInput(SDL_Event& event) override;
    std::string getID() override { return m_id; };
    void setDebug(bool debug) { isDebug = debug; }

private:
    bool isDebug = false;
    SDL_Rect camera;
    std::unique_ptr<Registry> registry;
    std::unique_ptr<StateLoader> stateLoader;
    const std::string m_id = "menu";
    std::vector<std::string> m_textureIds;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;
};

}
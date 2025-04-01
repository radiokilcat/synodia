#pragma once

#include "gamestate.h"
#include "stateLoader.hpp"
#include "../EventBus/EventBus.h"
#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../Render/IRenderer.hpp"

namespace anvil {

class PlayState : public GameState
{
public:
    PlayState();
    bool onEnter() override;
    bool onExit() override;

    void update(double deltaTime) override;
    void render(std::shared_ptr<IRenderer> renderer) override;
    void handleInput(SDL_Event& event) override;
    std::string getID() { return m_id; };
    void setDebug(bool debug) override { isDebug = debug; }

    std::unique_ptr<Registry>& getRegistry() { return registry; } 
    std::unique_ptr<AssetStore>& getAssetStore() { return assetStore; } 

private:
    std::unique_ptr<Registry> registry;
    std::unique_ptr<StateLoader> stateLoader;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;
    const std::string m_id = "play";
    bool isDebug = false;
    SDL_Rect camera;
};

}
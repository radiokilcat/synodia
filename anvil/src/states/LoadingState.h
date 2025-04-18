#pragma once

#include "gamestate.h"
#include "stateLoader.hpp"
#include "../EventBus/EventBus.h"
#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../Render/IRenderer.hpp"
#include "game_state_machine.h"
#include "ILoadable.hpp"

namespace anvil {

class LoadingState : public GameState
{
public:
    LoadingState(std::unique_ptr<ILoadableState> state);
    bool onEnter() override;
    bool onExit() override;

    void update(double deltaTime) override;
    void render(std::shared_ptr<IRenderer> renderer) override;
    void handleInput(SDL_Event& event) override;
    std::string getID() { return m_id; };
    void setDebug(bool debug) override { isDebug = debug; }

    void loadAssetsStepByStep();

    std::unique_ptr<Registry>& getRegistry() { return registry; } 
    std::unique_ptr<AssetStore>& getAssetStore() { return assetStore; } 

private:
    std::unique_ptr<Registry> registry;
    std::unique_ptr<StateLoader> stateLoader;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;
    std::shared_ptr<ITexture> text;
    std::unique_ptr<ILoadableState> stateToLoad;
    std::vector<AssetRequest> assetsToLoad;


    GameState* nextState;
    const std::string m_id = "load";
    size_t currentAssetIndex = 0;
    bool loaded = false;
    float progress = 0.0f;
    bool isDebug = false;
    SDL_Rect camera;

};


}
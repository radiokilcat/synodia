#include <nlohmann/json.hpp>
#include "LoadingState.h"
#include "../Logger/Logger.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../AssetStore/AssetStore.h"
#include "../Application.h"
#include "../systems/MovementSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/AnimationSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/RenderColliderSystem.h"
#include "../systems/DamageSystem.h"
#include "../systems/KeyboardControlSystem.h"
#include "../systems/CameraMovementSystem.h"
#include "../systems/ProjectileEmitSystem.h"
#include "../systems/ProjectileLifecycleSystem.h"
#include "../systems/RenderTextSystem.h"
#include "../systems/RenderHealthBarSystem.h"
#include "../systems/RenderImGUISystem.h"
#include "../systems/ButtonSystem.h"
#include "../systems/RenderTileMapSystem.h"

namespace anvil {

LoadingState::LoadingState(std::unique_ptr<ILoadableState> state)
        : registry(std::make_unique<Registry>())
        , assetStore(std::make_unique<AssetStore>())
        , stateToLoad(std::move(state))
        , eventBus(std::make_unique<EventBus>()) {

    stateLoader = std::make_unique<StateLoader>(registry);
}

bool LoadingState::onEnter() {
    Logger::Log("Enter Loading state");

    assetsToLoad = stateToLoad->getAssetsToLoad();
    registry->AddSystem<RenderTextSystem>();

    Entity newEntity = registry->CreateEntity();
    assetStore->AddFont("vera", "assets/fonts/VeraMono.ttf", 24);
    SDL_Color color = { 255, 255, 255, 255 };

    Entity loadingText = registry->CreateEntity();
    loadingText.AddComponent<TextLabelComponent>( glm::vec2(50, 50), "Loading...",
    "vera", SDL_Color{255, 255, 255, 255}, false, true);
    loadingText.Tag("loading-text");

    Entity currentFile = registry->CreateEntity();
    currentFile.AddComponent<TextLabelComponent>( glm::vec2(50, 90), "",
    "vera", SDL_Color{255, 255, 255, 255}, false, true);
    currentFile.Tag("current-file");

    currentAssetIndex = 0;
    progress = 0.0f;
    return true;
}

bool LoadingState::onExit() {
    Logger::Log("Exit Loading state");
    return true;
}

void LoadingState::update(double deltaTime) {
    registry->Update();
    if (progress >= 1.0f) {
        stateToLoad->onAssetsLoaded(std::move(*assetStore));
        auto state = stateToLoad.release();
        auto game_state = dynamic_cast<GameState*>(state);
        Application::Instance()->getStateMachine()->changeState(game_state);
        return;
    }

    loadAssetsStepByStep();
}

void LoadingState::render(std::shared_ptr<IRenderer> renderer) {
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
    SDL_FRect barBg = { 50.0f, 120.0f, 300.0f, 25.0f };
    renderer->fillRect(barBg, {255, 255, 255, 255});
    SDL_FRect barFill = barBg;
    barFill.w *= progress;
    renderer->fillRect(barFill, {100, 100, 100, 255});
}

void LoadingState::handleInput(SDL_Event& event) {
}

void LoadingState::loadAssetsStepByStep() {
    auto renderer = Application::Instance()->getRenderer();

    if (currentAssetIndex >= assetsToLoad.size()) {
        loaded = true;
        return;
    }

    const auto& req = assetsToLoad[currentAssetIndex];
    switch (req.type) {
        case AssetRequest::Type::Texture:
            assetStore->AddTexture(renderer, req.id, req.path);
            break;
        case AssetRequest::Type::Font:
            assetStore->AddFont(req.id, req.path, 24);
            break;
    }

    ++currentAssetIndex;
    registry->GetEntityByTag("current-file").GetComponent<TextLabelComponent>().text = req.path.c_str();
    progress = static_cast<float>(currentAssetIndex) / assetsToLoad.size();
}


}
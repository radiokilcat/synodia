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

LoadingState::LoadingState(GameState *nextState)
        : registry(std::make_unique<Registry>())
        , assetStore(std::make_unique<AssetStore>())
        , nextState(nextState)
        , eventBus(std::make_unique<EventBus>()) {

    stateLoader = std::make_unique<StateLoader>(registry);
}

bool LoadingState::onEnter() {
    Logger::Log("Enter Loading state");
    registry->AddSystem<RenderTextSystem>();
    auto renderer = Application::Instance()->getRenderer();

    auto data = stateLoader->readJsonContent("assets/objects/lady_textures.json");
    for (auto& [key, value] : data["textures"].items()) {
        textureQueue.push_back({ key, value.get<std::string>() });
    }
    // text = renderer->createTextTexture("Loading...", nullptr, {255, 255, 255, 255});
    assetStore->AddFont("vera", "assets/fonts/VeraMono.ttf", 24);

    Entity newEntity = registry->CreateEntity();
    SDL_Color color = { 255, 255, 255, 255 };
    newEntity.AddComponent<TextLabelComponent>( glm::vec2(50, 50), "Loading...",
    "vera", SDL_Color{255, 255, 255, 255}, false, true);
    Logger::Log("Entity created with ID: {}", newEntity.GetId());
    Logger::Log("Entity has component: {}", newEntity.HasComponent<TextLabelComponent>());
    currentIndex = 0;
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
        Application::Instance()->getStateMachine()->changeState(nextState);
        return;
    }

    const auto& [key, value] = textureQueue[currentIndex];
    auto renderer = Application::Instance()->getRenderer();
    assetStore->AddTexture(renderer, key, value);
    currentIndex++;
    progress = static_cast<float>(currentIndex) / textureQueue.size();
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

}
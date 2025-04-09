#include "playstate.h"
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

PlayState::PlayState()
        : registry(std::make_unique<Registry>())
        , assetStore(std::make_unique<AssetStore>())
        , eventBus(std::make_unique<EventBus>()) {

    stateLoader = std::make_unique<StateLoader>(registry);
}

bool PlayState::onEnter() {
    Logger::Log("Enter Play state");
    {
        registry->AddSystem<MovementSystem>();
        registry->AddSystem<RenderSystem>();
        registry->AddSystem<AnimationSystem>();
        registry->AddSystem<CollisionSystem>();
        registry->AddSystem<RenderColliderSystem>();
        registry->AddSystem<DamageSystem>();
        registry->AddSystem<KeyboardControlSystem>();
        registry->AddSystem<CameraMovementSystem>();
        registry->AddSystem<ProjectileEmitSystem>();
        registry->AddSystem<ProjectileLifecycleSystem>();
        registry->AddSystem<RenderTextSystem>();
        registry->AddSystem<RenderHealthBarSystem>();
        registry->AddSystem<RenderImGUISystem>();
        registry->AddSystem<ButtonSystem>();
        registry->AddSystem<RenderTileMapSystem>();
    }

    camera.x = 0;
    camera.y = 0;
    camera.w = Application::Instance()->getLogicalWidth();
    camera.h = Application::Instance()->getLogicalHeight();
    auto renderer = Application::Instance()->getRenderer();
    stateLoader->loadResources(renderer, "assets/objects/play_resources.json", assetStore);
    stateLoader->loadResources(renderer, "assets/objects/lady_textures.json", assetStore);
    stateLoader->loadFromFile("assets/objects/play.json");
    return true;
}

bool PlayState::onExit() {
    Logger::Log("Exit Play state");
    return true;
}

void PlayState::update(double deltaTime) {
    eventBus->Reset();

    registry->GetSystem<ButtonSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

    registry->Update();
    registry->GetSystem<AnimationSystem>().Update(assetStore);
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
    
    registry->Update();
}

void PlayState::render(std::shared_ptr<IRenderer> renderer) {
    registry->GetSystem<RenderTileMapSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetStore, camera);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
        // registry->GetSystem<RenderImGUISystem>().Update(registry, camera);
    }
}

void PlayState::handleInput(SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
            eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            eventBus->EmitEvent<MouseMotionEvent>(event.motion.x, event.motion.y);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            eventBus->EmitEvent<MouseClickedEvent>(event.button.x, event.button.y, event.button.button);
            break;
    }
}

}
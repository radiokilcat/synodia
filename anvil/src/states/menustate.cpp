#include "menustate.h"
#include "playstate.h"
#include "application.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthBarSystem.h"
#include "../Systems/RenderImGUISystem.h"
#include "../Systems/ButtonSystem.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/MouseEvents.h"

#include "../Logger/Logger.h"

namespace anvil {
    MenuState::MenuState()
        : registry(std::make_unique<Registry>())
        , assetStore(std::make_unique<AssetStore>())
        , eventBus(std::make_unique<EventBus>())
    {
        stateLoader = std::make_unique<StateLoader>(registry);
    }

bool MenuState::onEnter() {
    Logger::Log("Enter Menu state");
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderImGUISystem>();
    registry->AddSystem<ButtonSystem>();

    camera.x = 0;
    camera.y = 0;
    camera.w = Application::Instance()->getLogicalWidth();
    camera.h = Application::Instance()->getLogicalHeight();
    auto renderer = Application::Instance()->getRenderer();
    stateLoader->loadResources(renderer, "assets/objects/menu_textures.json", assetStore);
    stateLoader->loadFromFile("assets/objects/menu.json");
    return true;
}

void MenuState::render(std::shared_ptr<IRenderer> renderer) {
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
    if (isDebug) {
        registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
    }
}

bool MenuState::onExit() {
    Logger::Log("Exit Menu state");
    return true;
}

void MenuState::update(double deltaTime) {
    eventBus->Reset();
    registry->GetSystem<ButtonSystem>().SubscribeToEvents(eventBus);
    registry->Update();
    registry->GetSystem<AnimationSystem>().Update(assetStore);
}

void MenuState::handleInput(SDL_Event& event) {
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
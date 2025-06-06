#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/MouseEvents.h"
#include "../Events/ButtonEvents.h"
#include "../components/SpriteComponent.h"
#include "../components/TransformComponent.h"
#include "../Logger/Logger.h"
#include "../Application.h"
#include "../states/game_state_machine.h"
#include "../states/playstate.h"
#include "../states/LoadingState.h"

namespace anvil {

class ButtonSystem: public System {
    public:
        ButtonSystem() {
            RequireComponent<SpriteComponent>();
            RequireComponent<TransformComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<MouseMotionEvent>(this, &ButtonSystem::onMouseMotion);
            eventBus->SubscribeToEvent<MouseClickedEvent>(this, &ButtonSystem::onMouseClicked);
            eventBus->SubscribeToEvent<ButtonClickedEvent>([](ButtonClickedEvent& event) {
                Logger::Log("Button clicked event received");
                if (event.entity.HasTag("play-button")) {
                    Logger::Log("Play button clicked in event");
                    GameState* LoadState = new LoadingState(std::make_unique<PlayState>());
                    Application::Instance()->getStateMachine()->changeState(LoadState);
                } else if (event.entity.HasTag("exit-button")) {
                    Logger::Log("Exit button clicked in event");
                }
            });
        }

        void onMouseMotion(MouseMotionEvent& event) {
            for (auto entity: GetSystemEntities()) {
                auto& sprite = entity.GetComponent<SpriteComponent>();
                if (entity.HasTag("play-button") || entity.HasTag("exit-button")) {
                    if (isMouseOverButton(event.x, event.y, sprite, entity.GetComponent<TransformComponent>())) {
                            Logger::Log("Mouse over button");
                    }
                }
                if (entity.HasTag("start") && isMouseOverButton(event.x, event.y, sprite, entity.GetComponent<TransformComponent>()) ) {
                    entity.GetComponent<SpriteComponent>().srcRect.x = 200;
                    Logger::Log("Mouse over button start");
                }
            }
        }

        bool isMouseOverButton(int x, int y, SpriteComponent& sprite, TransformComponent& transform) {
            return x >= transform.position.x && x <= transform.position.x + sprite.width &&
                   y >= transform.position.y && y <= transform.position.y + sprite.height;
        }

        void onMouseClicked(MouseClickedEvent& event) {
            for (auto entity: GetSystemEntities()) {
                auto& sprite = entity.GetComponent<SpriteComponent>();
                auto& transform = entity.GetComponent<TransformComponent>();
                if (isMouseOverButton(event.x, event.y, sprite, transform)) {
                    if (entity.HasTag("play-button")) {
                        Logger::Log("Play button clicked");
                        GameState* LoadState = new LoadingState(std::make_unique<PlayState>());
                        Application::Instance()->getStateMachine()->changeState(LoadState);
                    } else if (entity.HasTag("exit-button")) {
                        Logger::Log("Exit button clicked");
                    }
                }
            }
        }

        void Update() {
                
        }
};

}
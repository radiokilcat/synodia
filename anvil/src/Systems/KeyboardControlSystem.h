#pragma once

#include <fmt/format.h>

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../components/KeyboardControlledComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/AnimationComponent.h"

namespace anvil {

class KeyboardControlSystem: public System {
    public:
        KeyboardControlSystem() {
            RequireComponent<KeyboardControlledComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        std::string directionToAngle(direction dir) {
            switch (dir) {
                case direction::UP:
                    return "000";
                case direction::RIGHT:
                    return "090";
                case direction::DOWN:
                    return "180" ;
                case direction::LEFT:
                    return "270" ;
            }
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
        }
        std::string lastAnimation;

        void OnKeyPressed(KeyPressedEvent& event) {
            for (auto entity: GetSystemEntities()) {
                const auto keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();
                auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
                auto& animation = entity.GetComponent<AnimationComponent>();

                switch (event.symbol) {
                    case SDLK_UP:
                        rigidbody.velocity = keyboardcontrol.upVelocity;
                        sprite.assetId = "lady_run_bow_body_000" ;
                        sprite.dir = direction::UP;
                        break;
                    case SDLK_RIGHT:
                        rigidbody.velocity = keyboardcontrol.rightVelocity;
                        sprite.assetId = "lady_run_bow_body_090" ;
                        sprite.dir = direction::RIGHT;
                        break;
                    case SDLK_DOWN:
                        rigidbody.velocity = keyboardcontrol.downVelocity;
                        sprite.assetId = "lady_run_bow_body_180" ;
                        sprite.dir = direction::DOWN;
                        break;
                    case SDLK_LEFT:
                        rigidbody.velocity = keyboardcontrol.leftVelocity;
                        sprite.assetId = "lady_run_bow_body_270" ;
                        sprite.dir = direction::LEFT;
                        break;
                    case SDLK_SPACE:
                        rigidbody.velocity = {0, 0};
                        sprite.assetId = fmt::format("{}_{}", "lady_attack_bow_body", directionToAngle(sprite.dir));
                        break;
                }

            }
        }

        void Update() {
                
        }
};

}
#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include <SDL3/SDL.h>

namespace anvil {

class RenderColliderSystem: public System {
    public:
        RenderColliderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update(std::shared_ptr<IRenderer> renderer, SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto collider = entity.GetComponent<BoxColliderComponent>();

                SDL_FRect colliderRect = {
                    transform.position.x + collider.offset.x - camera.x,
                    transform.position.y + collider.offset.y - camera.y,
                    static_cast<float>(collider.width) * transform.scale.x,
                    static_cast<float>(collider.height) * transform.scale.y
                };
                // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                // SDL_RenderRect(renderer, &colliderRect);
            }
        }
};

}
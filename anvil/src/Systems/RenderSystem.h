#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL3/SDL.h>

#include <cmath>

bool approximatelyEqual(float a, float b, float relativeEpsilon = 1e-5f) {
    if (std::fabs(a) < relativeEpsilon || std::fabs(b) < relativeEpsilon)
        return std::fabs(a - b) < relativeEpsilon;
    return std::fabs(a - b) / std::max(std::fabs(a), std::fabs(b)) < relativeEpsilon;
}

namespace anvil {

class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {
            struct RenderableEntity {
                TransformComponent transformComponent;
                SpriteComponent spriteComponent;
            };
            std::vector<RenderableEntity> renderableEntities; 
            for (auto entity: GetSystemEntities()) {
                RenderableEntity renderableEntity;
                renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
                renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();

                const float EPSILON = 1e-5f;
                bool isOutsideCameraView = (
                    renderableEntity.transformComponent.position.x + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.x - EPSILON ||
                    renderableEntity.transformComponent.position.x > camera.x + camera.w + EPSILON ||
                    renderableEntity.transformComponent.position.y + (renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height) < camera.y - EPSILON ||
                    renderableEntity.transformComponent.position.y > camera.y + camera.h + EPSILON
                );

                if (isOutsideCameraView && !renderableEntity.spriteComponent.isFixed) {
                    continue;
                }
                renderableEntities.emplace_back(renderableEntity);

            }

            std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
            });

            for (auto entity: renderableEntities) {
                const auto transform = entity.transformComponent;
                const auto sprite = entity.spriteComponent;

                SDL_FRect srcRect = sprite.srcRect;

                SDL_FRect dstRect = {
                    transform.position.x - (sprite.isFixed ? 0.f : static_cast<float>(camera.x)),
                    transform.position.y - (sprite.isFixed ? 0.f : static_cast<float>(camera.y)),
                    static_cast<float>(sprite.width) * transform.scale.x,
                    static_cast<float>(sprite.height) * transform.scale.y
                };

                SDL_RenderTextureRotated(
                    renderer,
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation,
                    NULL,
                    sprite.flip
                );
            }
        }
};

}
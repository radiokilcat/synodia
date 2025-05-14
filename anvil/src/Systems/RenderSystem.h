#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include "../Render/IRenderer.hpp"
#include <algorithm>
#include <SDL3/SDL.h>
#include "../Render/SDL/SDL_Renderer.hpp"

namespace anvil {

class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(std::shared_ptr<IRenderer> renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {
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

                // Draw green outline
                // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                // SDL_RenderRect(renderer, &dstRect);
                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                if (auto r = dynamic_cast<SDLRenderer*>(renderer.get())) {
                    r->DrawGreenOutlineRect(dstRect, {0, 255, 0, 255});
                }

                auto texture = assetStore->GetTexture(sprite.assetId);

                renderer->renderTextureRotated(
                    texture.get(), &srcRect, &dstRect, transform.rotation, nullptr, sprite.flip
                );
            }
        }
};

}
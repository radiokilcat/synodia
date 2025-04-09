#pragma once

#include "../ECS/ECS.h"
#include "../components/SpriteComponent.h"
#include "../components/AnimationComponent.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"
#include "../Render/ITexture.hpp"
#include <SDL3/SDL.h>

namespace anvil {

class AnimationSystem: public System {
    public:
        AnimationSystem() {
            RequireComponent<SpriteComponent>();
            RequireComponent<AnimationComponent>();
        }

        void Update(std::unique_ptr<AssetStore>& assetStore) {
            for (auto entity: GetSystemEntities()) {
                auto& animation = entity.GetComponent<AnimationComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();
                auto texture = assetStore->GetTexture(sprite.assetId);


                int columns = texture->getWidth() / sprite.width;
                int rows = texture->getHeight() / sprite.height;
                int totalFrames = columns * rows; 

                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % totalFrames;
                animation.frameX = animation.currentFrame % columns; 
                animation.frameY = animation.currentFrame / columns; 

                sprite.srcRect.x = animation.frameX * sprite.width;
                sprite.srcRect.y = animation.frameY * sprite.height;
            }
        }
};

}
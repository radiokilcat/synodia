#pragma once

#include "../ECS/ECS.h"
#include "../components/SpriteComponent.h"
#include "../components/AnimationComponent.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"
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

                int imageWidth;
                int imageHeight;
                // SDL_Texture* texture = assetStore->GetTexture(sprite.assetId);
                // SDL_QueryTexture(texture, NULL, NULL, &imageWidth, &imageHeight);


                int columns = imageWidth / sprite.width;
                int rows = imageHeight / sprite.height;
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
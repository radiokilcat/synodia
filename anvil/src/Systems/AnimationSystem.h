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
                SDL_Texture* texture = assetStore->GetTexture(sprite.assetId);
                SDL_QueryTexture(texture, NULL, NULL, &imageWidth, &imageHeight);
                Logger::Log("Image width: " + std::to_string(imageWidth) + ", Image height: " + std::to_string(imageHeight));


                int columns = imageWidth / sprite.width;
                int rows = imageHeight / sprite.height;
                int totalFrames = columns * rows; 
                Logger::Log("columns: " + std::to_string(columns) + ", rows: " + std::to_string(rows) + ", totalFrames: " + std::to_string(totalFrames));

                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % totalFrames;
                // Calculate row and column
                animation.frameX = animation.currentFrame % columns; 
                animation.frameY = animation.currentFrame / columns; 

                // Update the sprite’s source rectangle
                sprite.srcRect.x = animation.frameX * sprite.width;
                sprite.srcRect.y = animation.frameY * sprite.height;
            }
        }
};

}
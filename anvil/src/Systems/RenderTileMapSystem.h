#pragma once

#include "../ECS/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/TileComponent.h"
#include "../components/TileMapComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL3/SDL.h>
#include "../Application.h"
#include "../Logger/Logger.h"

namespace anvil {

class RenderTileMapSystem: public System {
    public:
        RenderTileMapSystem() {
            RequireComponent<TileMapComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {

        auto mapNumRows = 20;
        auto mapNumCols = 20;
        auto tileWidth = 128;
        auto tileHeight = 64;
        auto startX = Application::Instance()->getScreenWidth() / 2 - tileWidth / 2 - camera.x;
        auto startY = -100 - camera.y;
        int gridFloor = 0;
        int tileMargin = 10;
        std::string assetId = "stone_e";
        int w_, h_;
        SDL_QueryTexture(assetStore->GetTexture(assetId), NULL, NULL, &w_, &h_);
        Logger::Log("tile properties: " + std::to_string(w_) + " : " + std::to_string(h_));

        for (int y = 0; y < mapNumRows; y++) {
            for (int x = 0; x < mapNumCols; x++) {
                auto tileX = startX + (x - y) * tileWidth / 2;
                auto tileY = startY + (x + y) * tileHeight / 2;

                SDL_FRect destRect;
                SDL_FRect srcRect;
                srcRect.w = (float)w_;
                srcRect.h = (float)h_;
                srcRect.x = 0;
                srcRect.y = 0;

                int scaleFactor = srcRect.w / tileWidth;
                destRect.w = tileWidth;
                destRect.h = srcRect.h / scaleFactor;

                destRect.x = (float)tileX;
                destRect.y = (float)tileY - srcRect.h / scaleFactor + tileHeight + tileMargin;


                SDL_RenderTextureRotated(
                    renderer,
                    assetStore->GetTexture(assetId),
                    &srcRect,
                    &destRect,
                    NULL,
                    NULL,
                    SDL_FLIP_NONE
                );

            }
        }

        for (int y = 0; y < mapNumRows; y++) {
            for (int x = 0; x < mapNumCols; x++) {
                auto tileX = startX + (x - y) * tileWidth / 2;
                auto tileY = startY + (x + y) * tileHeight / 2;
                SDL_FPoint p1 = { tileX + tileWidth / 2, tileY };
                SDL_FPoint p2 = { tileX + tileWidth / 2 + tileWidth / 2, tileY  + tileHeight / 2 };
                SDL_FPoint p3 = { tileX + tileWidth / 2, tileY  + tileHeight };
                SDL_FPoint p4 = { tileX - tileWidth / 2 + tileWidth / 2, tileY + tileHeight / 2 };

                SDL_FPoint points[5] = { p1, p2, p3, p4, p1 };

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                SDL_RenderLines(renderer, points, 5);

                SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
            }
        }
    }
};

}
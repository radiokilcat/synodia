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

        void Update(std::shared_ptr<IRenderer> renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {

        auto mapNumRows = 20;
        auto mapNumCols = 20;
        auto tileWidth = 128;
        auto tileHeight = 64;
        auto startX = Application::Instance()->getScreenWidth() / 2 - tileWidth / 2 - camera.x;
        auto startY = -100 - camera.y;
        int gridFloor = 0;
        int tileMargin = 10;
        std::string assetId = "stone_e";
        auto tileTexture = assetStore->GetTexture(assetId);
        if (!tileTexture) {
            Logger::Err("RenderTileMapSystem: texture not found for assetId '{}'", assetId);
            return;
        }
        int w_ = tileTexture->getWidth();
        int h_ = tileTexture->getHeight();

        for (int y = 0; y < mapNumRows; y++) {
            for (int x = 0; x < mapNumCols; x++) {
                auto tileX = startX + (x - y) * tileWidth / 2;
                auto tileY = startY + (x + y) * tileHeight / 2;

                anvil::FRect destRect;
                anvil::FRect srcRect;
                srcRect.w = (float)w_;
                srcRect.h = (float)h_;
                srcRect.x = 0;
                srcRect.y = 0;

                int scaleFactor = static_cast<int>(srcRect.w / static_cast<float>(tileWidth));
                destRect.w = static_cast<float>(tileWidth);
                destRect.h = srcRect.h / scaleFactor;

                destRect.x = (float)tileX;
                destRect.y = (float)tileY - srcRect.h / scaleFactor + tileHeight + tileMargin;
                renderer->renderTextureRotated(
                    tileTexture.get(),
                    &srcRect,
                    &destRect,
                    0.0,
                    nullptr,
                    anvil::FlipMode::None
                );
        // for (int y = 0; y < mapNumRows; y++) {
        //     for (int x = 0; x < mapNumCols; x++) {
        //         auto tileX = startX + (x - y) * tileWidth / 2;
        //         auto tileY = startY + (x + y) * tileHeight / 2;
        //         SDL_FPoint p1 = { tileX + tileWidth / 2, tileY };
        //         SDL_FPoint p2 = { tileX + tileWidth / 2 + tileWidth / 2, tileY  + tileHeight / 2 };
        //         SDL_FPoint p3 = { tileX + tileWidth / 2, tileY  + tileHeight };
        //         SDL_FPoint p4 = { tileX - tileWidth / 2 + tileWidth / 2, tileY + tileHeight / 2 };

        //         SDL_FPoint points[5] = { p1, p2, p3, p4, p1 };

        //         SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        //         SDL_RenderLines(renderer, points, 5);

        //         SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
            }
        }
    }
};

}
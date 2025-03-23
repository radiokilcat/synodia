#pragma once

#include <string>
#include <SDL3/SDL.h>

namespace anvil {

struct TileComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    SDL_Rect srcRect;
    
    TileComponent(std::string assetId = "", int width = 0, int height = 0, int zIndex = 0, int srcRectX = 0, int srcRectY = 0) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->srcRect = {srcRectX, srcRectY, width, height};
    }
};


}
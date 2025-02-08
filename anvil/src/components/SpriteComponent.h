#pragma once

#include <string>
#include <SDL3/SDL.h>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    bool isFixed;
    SDL_RendererFlip flip;

    SDL_FRect srcRect;
    
    SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int zIndex = 0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->flip = SDL_FLIP_NONE;
        this->isFixed = isFixed;
        this->srcRect = {static_cast<float>(srcRectX), static_cast<float>(srcRectY),
             static_cast<float>(width), static_cast<float>(height)};
    }
};

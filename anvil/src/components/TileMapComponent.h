#pragma once

#include <string>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

namespace anvil {

struct TileMapComponent {
    int dimension;
    glm::vec2 startPosition;
    
    TileMapComponent(int dimension = 0, int startPosX = 0, int startPosY = 0) {
        this->dimension = dimension;
        this->startPosition.x = startPosX;
        this->startPosition.y = startPosY;
    }
};


}
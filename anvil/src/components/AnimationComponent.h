#pragma once

#include <SDL3/SDL.h>

struct AnimationComponent {
    int frameSpeedRate;
    bool isLoop;
    int currentFrame;
    int frameX = 0;
    int frameY = 0;
    uint8_t startTime;

    AnimationComponent(int frameSpeedRate = 1,
                       bool isLoop = true)
                        {
        this->currentFrame = 1;
        this->frameSpeedRate = frameSpeedRate;
        this->isLoop = isLoop;
        this->startTime = SDL_GetTicks();
    }
};

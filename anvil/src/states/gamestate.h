#pragma once

#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include "../ECS/ECS.h"


namespace anvil {

class GameState
{
public:
    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;

    virtual void update(double deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void handleInput(SDL_Event& event) = 0;
    virtual void setDebug(bool debug) = 0;
    
    virtual std::unique_ptr<Registry>& getRegistry() = 0;

    virtual std::string getID() = 0;
};

}

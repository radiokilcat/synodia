#pragma once

#include <string>
#include <vector>

#include "game_objects/gameobject.h"

namespace anvil {

class GameState
{
public:
    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;

    virtual void update(Uint64 deltaTime) = 0;
    virtual void render() = 0;

    virtual std::string getID() = 0;
};

}

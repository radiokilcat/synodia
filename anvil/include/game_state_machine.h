#pragma once

#include <vector>
#include "gamestate.h"


namespace anvil {

class GameStateMachine
{
public:
    void pushState(GameState* state);
    void popState();
    void changeState(GameState* state);
    GameState* previousState();
    void update();
    void render();
private:
    std::vector<GameState*> states_;
};


}

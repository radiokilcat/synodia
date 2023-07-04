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

    void update();
    void render();
private:
    std::vector<GameState*> states_;
};


}

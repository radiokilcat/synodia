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
    GameState* findState(std::string id);
    void update(Uint64 deltaTime);
    void render();
private:
    std::vector<GameState*> states_;
};


}

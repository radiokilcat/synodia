#include "game_state_machine.h"

namespace anvil {

void GameStateMachine::pushState(GameState* state)
{
    if (!states_.empty())
    {
        if (states_.back()->getID() == state->getID())
            return;
    }
    states_.push_back(state);
    state->onEnter();
}

void GameStateMachine::popState()
{
    if (!states_.empty())
    {
        if(states_.back()->onExit())
        {
            delete states_.back();
            states_.pop_back();
        }
    }
}

GameState* GameStateMachine::findState(std:: string id)
{
    if (states_.empty())
    {
        return nullptr;
    }
    auto state = std::find_if(states_.begin(), states_.end(), [&id](anvil::GameState *arg) {
        return (arg->getID() == id);
    });
    return *(state);
}

void GameStateMachine::changeState(GameState* state)
{
    if (!states_.empty())
    {
        if (states_.back()->getID() == state->getID())
            return;

        if(states_.back()->onExit())
        {
            delete states_.back();
            states_.pop_back();
        }
    }
    states_.push_back(state);
    state->onEnter();
}

void GameStateMachine::update()
{
    if (!states_.empty())
        states_.back()->update();
}

void GameStateMachine::render()
{
    if (!states_.empty())
        states_.back()->render();
}

}

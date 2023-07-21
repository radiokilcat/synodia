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
// TODO ѕредлагаю тут и далее заменить NULL на nullptr (плюсовый аналог)
GameState* GameStateMachine::previousState()
{
    if (!states_.empty())
    {
        return *(states_.end() - 2);
    }
    return nullptr;
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

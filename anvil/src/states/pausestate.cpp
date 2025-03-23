#include "pausestate.h"
#include "playstate.h"

#include "../Logger/Logger.h"

namespace anvil {

bool PauseState::onEnter() {
    Logger::Log("Enter Pause state");
    return stateLoader->loadFromFile("assets/states/pause_state.json");
}

bool PauseState::onExit() {
    Logger::Log("Exit Pause state");
    return true;
}

void PauseState::update(double deltaTime) {
}

void PauseState::render(SDL_Renderer* renderer) {
}

void PauseState::handleInput(SDL_Event& event) {
    // Handle pause state specific input
}

}
#include "losestate.h"
#include "playstate.h"

#include "../Logger/Logger.h"

namespace anvil {

bool LoseState::onEnter() {
    Logger::Log("Enter Lose state");
    return stateLoader->loadFromFile("assets/states/lose_state.json");
}

void LoseState::render(std::shared_ptr<IRenderer> renderer) {
}

bool LoseState::onExit() {
    Logger::Log("Exit Lose state");
    return true;
}

void LoseState::update(double deltaTime) {
}

void LoseState::handleInput(SDL_Event& event) {
    // Handle lose state specific input
}

}
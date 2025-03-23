#include "editstate.h"
#include "../Logger/Logger.h"

namespace anvil {


bool EditState::onEnter() {
    Logger::Log("Enter Edit state");
    return stateLoader->loadFromFile("assets/states/edit_state.json");
}

void EditState::render(SDL_Renderer* renderer) {
}

bool EditState::onExit() {
    Logger::Log("Exit Edit state");
    return true;
}

void EditState::update(double deltaTime) {
}

void EditState::handleInput(SDL_Event& event) {
    // Handle edit state specific input
}

}
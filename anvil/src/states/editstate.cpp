#include "editstate.h"
#include "../Logger/Logger.h"

namespace anvil {


bool EditState::onEnter() {
    Logger::Log("Enter Edit state");
    return stateLoader->loadFromFile("assets/states/edit_state.json");
}

void EditState::render(std::shared_ptr<IRenderer> renderer) {
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
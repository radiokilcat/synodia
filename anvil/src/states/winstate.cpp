#include "winstate.h"
#include "playstate.h"
#include "../Logger/Logger.h"

namespace anvil {

bool WinState::onEnter() {
    Logger::Log("Enter Win state");
    return stateLoader->loadFromFile("assets/states/win_state.json");
}

void WinState::render(std::shared_ptr<IRenderer> renderer) {
}

bool WinState::onExit() {
    Logger::Log("Exit Win state");
    return true;
}

void WinState::update(double deltaTime) {
}

void WinState::handleInput(SDL_Event& event) {
    // Handle win state specific input
}

}
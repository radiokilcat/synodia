#pragma once
#include "ICommand.hpp"
#include "../Application.h"
#include "../states/game_state_machine.h"
#include "../states/menustate.h"
#include "../states/playstate.h"
#include "../states/LoadingState.h"

namespace anvil {

struct DelayedSceneSwitchCommand : public ICommand {
    std::string sceneName;
    float delaySeconds;

    DelayedSceneSwitchCommand(const std::string& name, float delay)
        : sceneName(name), delaySeconds(delay) {}

    void update(float dt) override {
        delaySeconds -= dt;
    }

    bool isReady() const override {
        return delaySeconds <= 0.0f;
    }

    void execute() override {
        GameState* LoadState = new LoadingState(std::make_unique<PlayState>());
        Application::Instance()->getStateMachine()->changeState(LoadState);
    }
};

}
#pragma once
#include "../ECS/ECS.h"

namespace anvil {

struct ICommand {
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual bool isReady() const { return true; }
    virtual void update(float dt) {}
};

}
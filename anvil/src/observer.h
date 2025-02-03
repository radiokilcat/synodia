#pragma once

#include <memory>

#include "game_objects/IGameObject.h"

namespace anvil {

class IObserver {
public:
    virtual ~IObserver() = 0;
    virtual void onNotify(std::weak_ptr<IGameObject> obj);
};

}

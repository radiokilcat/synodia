#pragma once

#include "observer.h"
#include "game_objects/IGameObject.h"

namespace anvil {

class GameObjectsObserver : IObserver {
public:
    void onNotify(std::weak_ptr<IGameObject>);
};

}

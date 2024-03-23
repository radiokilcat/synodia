#pragma once

#include "game_objects/BaseGameObject.h"

namespace anvil {

class Observer {
public:
    virtual ~Observer() = 0;
    virtual void onNotify(BaseGameObject* obj);
};

}

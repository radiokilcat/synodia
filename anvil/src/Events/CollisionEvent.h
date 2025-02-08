#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

namespace anvil {

class CollisionEvent: public Event {
    public:
        Entity a;
        Entity b;
        CollisionEvent(Entity a, Entity b): a(a), b(b) {}
};

}

#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

namespace anvil {

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class PlayerMoveEvent: public Event {
    public:
        Direction direction;
        Entity entity;
        PlayerMoveEvent(Direction direction, Entity entity): direction(direction), entity(entity) {}
};

class PlayerStoppedEvent: public Event {
    public:
        Entity entity;
        PlayerStoppedEvent(Entity entity): entity(entity) {}
};

}

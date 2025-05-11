#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

namespace anvil {

class ButtonClickedEvent : public Event {
public:
    Entity entity;

    ButtonClickedEvent(Entity entity)
        : entity(entity) {}
};

class ButtonHoverEvent : public Event {
public:
    int x;
    int y;

    ButtonHoverEvent(int x, int y)
        : x(x), y(y) {}
};

class ButtonMotionEvent : public Event {
public:
    int x;
    int y;

    ButtonMotionEvent(int x, int y)
        : x(x), y(y) {}
};

}
#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include <SDL3/SDL.h>

namespace anvil {

class MouseClickedEvent : public Event {
public:
    int x;
    int y;
    Uint8 button;

    MouseClickedEvent(int x, int y, Uint8 button)
        : x(x), y(y), button(button) {}
};

class MouseHoverEvent : public Event {
public:
    int x;
    int y;

    MouseHoverEvent(int x, int y)
        : x(x), y(y) {}
};

class MouseMotionEvent : public Event {
public:
    int x;
    int y;

    MouseMotionEvent(int x, int y)
        : x(x), y(y) {}
};

}
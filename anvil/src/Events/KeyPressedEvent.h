#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include <SDL3/SDL.h>

namespace anvil {

class KeyPressedEvent: public Event {
    public:
        SDL_Keycode symbol;
        KeyPressedEvent(SDL_Keycode symbol): symbol(symbol) {}
};


}
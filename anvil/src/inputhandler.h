#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_joystick.h>

#include <vector>

#include "vector2d.h"
#include "EventBus/EventBus.h"

namespace anvil {

enum mouse_buttons
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

enum class AnvilKeyCode {
    Up,
    Down,
    Left,
    Right,
    Space,
    Escape,
    Return,
    Unknown
};

class InputHandler
{
public:
    static InputHandler* instance();
    void handleEvents();
    void clean();

    bool getMouseButtonState(int buttonNumber);
    Vector2D* getMousePosition() const;

    bool isKeyDown(AnvilKeyCode key);
    bool isKeyUp(AnvilKeyCode key);
    bool isAnyKeyUp();
    bool isAnyKeyDown();

private:
    InputHandler();
    ~InputHandler();
    std::vector<std::vector<bool>> m_buttonStates;
    std::vector<bool> m_mouseButtonStates;
    Vector2D* m_mousePosition;

    const Uint8* m_keyState;
    std::vector<Uint8> m_releasedKeys;
    bool anyKeyPressed_ = false;
    std::unique_ptr<EventBus> eventBus;
};


}

#include <iostream>
#include <backends/imgui_impl_sdl3.h>

#include "inputhandler.h"
#include "application.h"


namespace anvil {

SDL_Scancode anvilToSDLKey(AnvilKeyCode key) {
    switch (key) {
    case AnvilKeyCode::Up: return SDL_SCANCODE_W;
    case AnvilKeyCode::Down: return SDL_SCANCODE_S;
    case AnvilKeyCode::Left: return SDL_SCANCODE_A;
    case AnvilKeyCode::Right: return SDL_SCANCODE_D;
    case AnvilKeyCode::Space: return SDL_SCANCODE_SPACE;
    case AnvilKeyCode::Escape: return SDL_SCANCODE_ESCAPE;
    case AnvilKeyCode::Return: return SDL_SCANCODE_RETURN;

    default: return SDL_SCANCODE_UNKNOWN;
    }

}

bool InputHandler::getMouseButtonState(int buttonNumber)
{
    return m_mouseButtonStates[buttonNumber];
}

bool InputHandler::isKeyDown(AnvilKeyCode key) {
    return m_keyState[anvilToSDLKey(key)];
}

bool InputHandler::isKeyUp(AnvilKeyCode key) {
    return m_releasedKeys[anvilToSDLKey(key)];
}
    
bool InputHandler::isAnyKeyUp() {
    return std::count(m_releasedKeys.begin(), m_releasedKeys.end(), 1) > 0;
}
    
bool InputHandler::isAnyKeyDown() {
    return anyKeyPressed_;
}
    
static InputHandler* instance_;
InputHandler *InputHandler::instance()
{
    if (instance_ == 0)
    {
        instance_ = new InputHandler();
    }
    return instance_;
}

void InputHandler::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        m_keyState = SDL_GetKeyboardState(0);
        m_releasedKeys = std::vector<Uint8>(512, 0);

#ifndef NDEBUG
        ImGui_ImplSDL3_ProcessEvent(&event);
#endif

        if (event.type == SDL_EVENT_QUIT) {
            Application::Instance()->quit();
        }
        if (event.type == SDL_EVENT_KEY_UP) {
            m_releasedKeys[event.key.keysym.scancode] = 1;
        }
        if (event.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.button.button == SDL_BUTTON_LEFT)
                m_mouseButtonStates[LEFT] = true;
            if (event.button.button == SDL_BUTTON_MIDDLE)
                m_mouseButtonStates[MIDDLE] = true;
            if (event.button.button == SDL_BUTTON_RIGHT)
                m_mouseButtonStates[RIGHT] = true;
        }
        if (event.button.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
                m_mouseButtonStates[LEFT] = false;
            if (event.button.button == SDL_BUTTON_MIDDLE)
                m_mouseButtonStates[MIDDLE] = false;
            if (event.button.button == SDL_BUTTON_RIGHT)
                m_mouseButtonStates[RIGHT] = false;
        }
        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            m_mousePosition->setX(event.motion.x);
            m_mousePosition->setY(event.motion.y);
        }
    }
    m_keyState = SDL_GetKeyboardState(0);

}

void InputHandler::clean()
{
}

InputHandler::InputHandler()
    : m_mousePosition{new Vector2D{0.0f, 0.0f}}, m_keyState(0)
{
    for (int i = 0; i < 3; ++i)
    {
        m_mouseButtonStates.push_back(false);
    }
}

InputHandler::~InputHandler() {}

Vector2D* InputHandler::getMousePosition() const
{
    return m_mousePosition;
}

}


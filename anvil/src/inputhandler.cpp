#include <iostream>

#include "inputhandler.h"
#include "application.h"


namespace anvil {

SDL_Scancode anvilToSDLKey(AnvilKeyCode key) {
    switch (key) {
    case AnvilKeyCode::Up: return SDL_SCANCODE_UP;
    case AnvilKeyCode::Down: return SDL_SCANCODE_DOWN;
    case AnvilKeyCode::Left: return SDL_SCANCODE_LEFT;
    case AnvilKeyCode::Right: return SDL_SCANCODE_RIGHT;
    case AnvilKeyCode::Space: return SDL_SCANCODE_SPACE;

    default: return SDL_SCANCODE_UNKNOWN;
    }

}

bool InputHandler::getMouseButtonState(int buttonNumber)
{
    return m_mouseButtonStates[buttonNumber];
}

bool InputHandler::isKeyDown(AnvilKeyCode key)
{
//    std::cout << "scan key " <<  std::endl;
//    switch (key) {
//    case AnvilKeyCode::Up: std::cout << "up" << std::endl;
//    case AnvilKeyCode::Down: std::cout << "down" << std::endl;
//    case AnvilKeyCode::Left: std::cout << "left" << std::endl;
//    case AnvilKeyCode::Right: std::cout << "Right" << std::endl;
//    case AnvilKeyCode::Space: std::cout << "Space" << std::endl;
//    }

    return m_keyState[anvilToSDLKey(key)];

}

//bool InputHandler::isKeyDown(SDL_Scancode code)
//{
//    if (m_keyState != 0)
//    {
//        if (m_keyState[code] == 1)
//            return true;
//        else
//            return false;
//    }
//    return false;
//}

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
        if (m_keyState[SDL_SCANCODE_UP]) {
            std::cout << "<UP> is pressed << std::endl;" << std::endl;
            }
        if (m_keyState[SDL_SCANCODE_RIGHT]) {
            std::cout << "<RIGHT> is pressed << std::endl;" << std::endl;
        }
        if (event.type == SDL_EVENT_QUIT)
        {
            Application::Instance()->quit();
        }
        if (event.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
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
        if (event.type = SDL_EVENT_MOUSE_MOTION)
        {
            m_mousePosition->setX(event.motion.x);
            m_mousePosition->setY(event.motion.y);
        }
    }

}

void InputHandler::clean()
{
}

InputHandler::InputHandler()
    : m_mousePosition{new Vector2D{0.0, 0.0}}
{
    for (int i = 0; i < 3; ++i)
    {
        m_mouseButtonStates.push_back(false);
    }
}

Vector2D* InputHandler::getMousePosition() const
{
    return m_mousePosition;
}

}


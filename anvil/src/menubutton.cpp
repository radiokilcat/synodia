#include "menubutton.h"

#include "inputhandler.h"
#include <iostream>

namespace anvil {

MenuButton::MenuButton(const LoaderParams *params, std::function<void()> callback)
    : GameObject(params),
    m_callback(callback)
{
    currentFrame_ = MOUSE_OUT;
}

void MenuButton::draw(std::shared_ptr<Renderer> renderer)
{
    GameObject::draw(renderer);
}

void MenuButton::update()
{
    Vector2D* MousePos = InputHandler::instance()->getMousePosition();
    if(MousePos->x() < (position_.x() + width_)
        && MousePos->x() > position_.x()
        && MousePos->y() < (position_.y() + height_)
        && MousePos->y() > position_.y())
    {
        if (InputHandler::instance()->getMouseButtonState(LEFT) && released_)
        {
            currentFrame_ = CLICKED;
            m_callback();
            released_ = false;
        }
        else if (!InputHandler::instance()->getMouseButtonState(LEFT))
        {
            released_ = true;
            currentFrame_ = MOUSE_OVER;
        }
    }
    else
    {
        currentFrame_ = MOUSE_OUT;
    }
}

void MenuButton::clean()
{
}

}

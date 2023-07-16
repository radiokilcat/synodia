#include "menubutton.h"
#include <iostream>


MenuButton::MenuButton(const anvil::LoaderParams* params, std::function<void()> callback)
    : anvil::GameObject(params),
    m_callback(callback)
{
    currentFrame_ = MOUSE_OUT;
}

void MenuButton::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::GameObject::draw(renderer);
}

void MenuButton::update()
{
    anvil::Vector2D* MousePos = anvil::InputHandler::instance()->getMousePosition();
    if(MousePos->x() < (position_.x() + width_)
        && MousePos->x() > position_.x()
        && MousePos->y() < (position_.y() + height_)
        && MousePos->y() > position_.y())
    {
        if (anvil::InputHandler::instance()->getMouseButtonState(anvil::LEFT) && released_)
        {
            currentFrame_ = CLICKED;
            m_callback();
            released_ = false;
        }
        else if (!anvil::InputHandler::instance()->getMouseButtonState(anvil::LEFT))
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

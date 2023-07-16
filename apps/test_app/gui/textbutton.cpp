#include "textbutton.h"


TextButton::TextButton(const anvil::LoaderParams* params,
                       std::string text,
                       std::function<void()> callback)
    : anvil::GameObject(params)
    , m_callback(callback)
    , m_text(text)
{
    m_label = new anvil::TextLabel("lbl", m_text,
                                   anvil::Color{54, 29, 50},
                                   position_.x(), position_.y(),
                                   width_, height_);
}

void TextButton::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::GameObject::draw(renderer);
    m_label->draw(renderer);
}

void TextButton::update()
{
    anvil::Vector2D* MousePos = anvil::InputHandler::instance()->getMousePosition();
    if(MousePos->x() < (position_.x() + width_)
        && MousePos->x() > position_.x()
        && MousePos->y() < (position_.y() + height_)
        && MousePos->y() > position_.y())
    {
        if (anvil::InputHandler::instance()->getMouseButtonState(anvil::LEFT) && m_released)
        {
            currentFrame_ = CLICKED;
            m_callback();
            m_released = false;
        }
        else if (!anvil::InputHandler::instance()->getMouseButtonState(anvil::LEFT))
        {
            m_released = true;
            currentFrame_ = MOUSE_OVER;
        }
    }
    else
    {
        currentFrame_ = MOUSE_OUT;
    }
}

void TextButton::clean()
{
}

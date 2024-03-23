#include "textbutton.h"
#include "logger.h"

TextButton::TextButton()
    : anvil::GameObject()
{
    currentFrame_ = MOUSE_OUT;
    m_label = new anvil::TextLabel(m_text, anvil::Color{54, 29, 50});
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

void TextButton::load(const anvil::LoaderParams* params)
{
    m_label->load(params);
    m_label->setText(m_text);
    GameObject::load(params);
}

bool TextButton::registerWithFactory()
{
    anvil::GameObjectFactory::instance().registerType("TextButton", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<TextButton>();
    });
    return true;
}

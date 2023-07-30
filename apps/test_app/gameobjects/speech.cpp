#include "speech.h"


Speech::Speech(const anvil::LoaderParams* params)
    : anvil::GameObject(params)
{
    m_label = new anvil::TextLabel("lbl", m_text,
        anvil::Color{0, 0, 0},
        position_.x() + 8, position_.y() + 8,
        width_, height_);
}

void Speech::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    if (isShown())
    {
        anvil::GameObject::drawScaled(renderer, 2.3);
        m_label->drawWrapped(renderer, 0);
    }
}

void Speech::update()
{
    // Cycle through the spreadsheet and change frame position
    currentFrame_ = isShown() ? SHOWN : HIDDEN;
    anvil::GameObject::update();
}

void Speech::setPosition(float x, float y)
{
    position_.setX(x);
    position_.setY(y);
    m_label->setPosition(position_.x() + 8, position_.y() + 8);
}

void Speech::show(int timeSec, std::string text)
{
    m_text = text;
    m_label->setText(m_text);
    shownAt = anvil::Application::Instance()->getTicks();
    showDuration = timeSec;
}
bool Speech::isShown() {
    return shownAt > 0 && showDuration > 0 &&
        anvil::Application::Instance()->getTicks() < shownAt + showDuration * 1000;
}
void Speech::clean()
{
}

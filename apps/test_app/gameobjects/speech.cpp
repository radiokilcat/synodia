#include "speech.h"


Speech::Speech()
    : anvil::GameObject()
{
    m_label = new anvil::TextLabel(m_text,
        anvil::Color{0, 0, 0});
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

void Speech::setSize(float w, float h)
{
    width_ = w;
    height_ = h;
}

void Speech::setTextSize(float size)
{
    m_label->setTextScale(size);
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

void Speech::load(const anvil::LoaderParams* params)
{
    GameObject::load(params);
    m_label->setText(m_text);
    m_label->setSize(params->getWidth(), params->getHeight());
    m_label->setPosition(position_.x() + 8, position_.y() + 8);
}

#include <SDL3/SDL.h>
#include <filesystem>
#include <string>

#include "vector2d.h"
#include "renderer.h"
#include "game_objects/textlabel.h"

namespace anvil {

TextLabel::TextLabel(std::string text,
                     Color color,
                     TTF_Font* font)
    : GameObject()
    , color_({ color.R, color.G, color.B })
    , text_(text)
    , font_(font)
{
}

void TextLabel::draw(std::shared_ptr<Renderer> renderer)
{
    TextureManager::instance()->drawText(id_, text_, font_, color_, position_.x(), position_.y(), width_, height_, renderer->getRenderer());
}

void TextLabel::drawWrapped(std::shared_ptr<Renderer> renderer, Uint32 wrapLength)
{
    TextureManager::instance()->drawTextWrapped(id_, wrapLength, text_, font_, color_, position_.x(), position_.y(), width_, height_, renderer->getRenderer());
};

void TextLabel::setPosition(float x, float y)
{
    position_.setX(x);
    position_.setY(y);
}

void TextLabel::setText(std::string text)
{
    text_ = text;
}

}

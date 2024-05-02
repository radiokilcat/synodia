#include "components/TextComponent.h"

#include "fontloader.h"
#include "components/Transform2DComponent.h"

namespace anvil {
	
SDL_Color getColor(const json& jsonData) {
    std::vector<int> colorValues = jsonData.value("color", std::vector<int>{255, 255, 255});
    SDL_Color color;
    color.r = colorValues.at(0);
    color.g = colorValues.at(1);
    color.b = colorValues.at(2);
    color.a = 255;
    return color;
}
    
TextComponent::TextComponent(std::string text, Color color, TTF_Font* font)
    : color_({ color.R, color.G, color.B })
    , text_(text)
    , font_(font)
{
}
    
TextComponent::TextComponent(const nlohmann::json& data) {
    color_ = getColor(data);
    text_ = data.value("text", "");
    width_ = data.value("width", 400);
    height_ = data.value("height", 100);
    font_ = FontLoader::instance()->getDefaultFont();
}
    
void TextComponent::draw(float x, float y, std::shared_ptr<Renderer> renderer) {
    TextureManager::instance()->drawText(text_, font_, color_, x, y, width_, height_, renderer->getRenderer());
}

void TextComponent::drawWrapped(float x, float y, std::shared_ptr<Renderer> renderer, Uint32 wrapLength)
{
    // TextureManager::instance()->drawTextWrapped(
    //     id_,
    //     wrapLength,
    //     text_,
    //     font_, 
    //     color_, 
    //     position_.x(),
    //     position_.y(), 
    //     std::min((int)width_ * textScale_, (int)text_.length() * 14),
    //     (int)height_ * textScale_,
    //     renderer->getRenderer());
};

void TextComponent::setPosition(float x, float y) {
    // position_.setX(x);
    // position_.setY(y);
}

void TextComponent::setSize(float w, float h) {
    width_ = w;
    height_ = h;
}

void TextComponent::setText(const std::string& text) {
    text_ = text;
}

void TextComponent::from_json(const nlohmann::json& j) {
    color_ = getColor(j);
    text_ = j.value("text", "");
    width_ = j.value("width", 200);
    height_ = j.value("height", 100);
    font_ = FontLoader::instance()->getDefaultFont();
}

void TextComponent::to_json(const nlohmann::json& j) {
    // j["text"] = text_;
}

void TextComponent::setTextScale(int size) {
    textScale_ = size;
}

}

#include <SDL3/SDL.h>
#include <filesystem>
#include <string>

#include "renderer.h"
#include "game_objects/textlabel.h"

#include "components/Transform2DComponent.h"
#include "components/Sprite2DComponent.h"
#include "components/TextComponent.h"
#include "game_objects/GameObjectsFactory.h"

namespace anvil {

TextLabel::TextLabel()
{
}
    
TextLabel::TextLabel(const nlohmann::json& data) {
}
    
void TextLabel::draw(std::shared_ptr<Renderer> renderer)
{
    auto x = transform_->getX();
    auto y = transform_->getY();
    text_->draw(x, y, renderer);
}

void TextLabel::drawWrapped(std::shared_ptr<Renderer> renderer, Uint32 wrapLength)
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

void TextLabel::setText(const std::string& text) {
    text_->setText(text);
}

void TextLabel::init() {
    if (!sprite_) {
        sprite_ = getComponent<anvil::Sprite2DComponent>();
    }
    if (!transform_) {
        transform_ = getComponent<anvil::Transform2DComponent>();
    }
    if (!text_) {
        text_ = getComponent<anvil::TextComponent>();
    }
}

void TextLabel::from_json(const nlohmann::json& j) {
    // id_ = j.value("id", std::string("test1"));
    // layer_ = j.value("layer", 0);
    // color_ = getColor(j);
    // text_ = j.value("text", "");
    // width_ = j.value("width", 200);
    // height_ = j.value("height", 100);
    // font_ = FontLoader::instance()->getDefaultFont();
}

void TextLabel::to_json(nlohmann::json& j) {
    j["id"] = id_;
    j["layer"] = layer_;
}

bool TextLabel::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("TextLabel", []() -> std::shared_ptr<anvil::IGameObject> {
        return std::make_shared<TextLabel>();
    });
    return true;
}
    
}

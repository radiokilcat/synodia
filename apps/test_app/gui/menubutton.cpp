#include "menubutton.h"

#include "components/Sprite2DComponent.h"


namespace anvil {
    class Sprite2DComponent;
}

void MenuButton::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    auto x = transform_->getX();
    auto y = transform_->getY();
    sprite_->draw(x, y, renderer);
    text_->draw(x, y, renderer);
}

void MenuButton::update(Uint64 deltaTime)
{
    if (isHovered()) {
        if (anvil::InputHandler::instance()->getMouseButtonState(anvil::LEFT) && released_) {
            sprite_->setCurrentFrame(CLICKED);
            if (m_callback)
                m_callback();
            released_ = false;
        }
        else if (!anvil::InputHandler::instance()->getMouseButtonState(anvil::LEFT)) {
            released_ = true;
            sprite_->setCurrentFrame(MOUSE_OVER);
        }
    }
    else {
        sprite_->setCurrentFrame(MOUSE_OUT);
    }
}

void MenuButton::init() {
    if (!sprite_) {
        sprite_ = getComponent<anvil::Sprite2DComponent>();
    }
    if (!transform_) {
        transform_ = getComponent<anvil::Transform2DComponent>();
    }
    if (!text_) {
        text_ = getComponent<anvil::TextComponent>();
    }
    sprite_->setCurrentFrame(MOUSE_OUT);
    sprite_->setCurrentRow(1);
}

void MenuButton::clean()
{
}

void MenuButton::from_json(const nlohmann::json& j) {
    id_ = j.value("id", std::string("test1"));
    layer_ = j.value("layer", 0);
    // color_ = getColor(j);
    // text_ = j.value("text", "");
}

void MenuButton::to_json(nlohmann::json& j) {
    j["id"] = id_;
    j["layer"] = layer_;
    // j["text"] = text_;
}

bool MenuButton::isHovered() {
    anvil::Vector2D* MousePos = anvil::InputHandler::instance()->getMousePosition();
    return MousePos->x() < (transform_->getX() + sprite_->width())
        && MousePos->x() > transform_->getX()
        && MousePos->y() < (transform_->getY() + sprite_->height())
        && MousePos->y() > transform_->getY();
}

bool MenuButton::registerWithFactory()
{
    anvil::GameObjectFactory::instance().registerType("Button", []() -> std::unique_ptr<anvil::IGameObject> {
        return std::make_unique<MenuButton>();
    });
    return true;
}

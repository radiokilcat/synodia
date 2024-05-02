#pragma once

#include <functional>
#include "anvil.h"
#include "components/Sprite2DComponent.h"
#include "components/TextComponent.h"

class MenuButton : public anvil::GameObject
{
public:
    MenuButton() = default;

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update(Uint64 deltaTime) override;
    void init() override;
    void clean() override;

    void setCallback(std::function<void()> callback) { m_callback = callback; };
    void setText(std::string text) { text_->setText(text); };
    void from_json(const nlohmann::json& j);
    void to_json(nlohmann::json& j);
    bool isHovered();

    static bool registerWithFactory();

private:

    enum button_states {
        MOUSE_OUT = 0,
        MOUSE_OVER = 1,
        CLICKED = 2
    };
    std::shared_ptr<anvil::Sprite2DComponent> sprite_;
    std::shared_ptr<anvil::Transform2DComponent> transform_;
    std::shared_ptr<anvil::TextComponent> text_;
    std::function<void()> m_callback;
    bool released_;
};

#pragma once

#include <functional>
#include <string>
#include "anvil.h"


class TextButton: public anvil::GameObject
{
public:
    TextButton();

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;
    void load(const anvil::GameObjectData *params) override;

    void setText(std::string text) { m_text = text; };
    void setCallback(std::function<void()> callback) override { m_callback = callback; };

    static bool registerWithFactory();

private:

    enum button_states
    {
        MOUSE_OUT = 0,
        MOUSE_OVER = 1,
        CLICKED = 2
    };
    std::function<void()> m_callback;
    std::string m_text;
    anvil::TextLabel* m_label = nullptr;

    bool m_released;

};

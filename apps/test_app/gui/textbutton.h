#pragma once

#include <functional>
#include <string>
#include "anvil.h"


class TextButton: public anvil::GameObject
{
public:
    TextButton();

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update(Uint64 deltaTime) override;
    void clean() override;
    void load();
    void init() {};

    void setText(std::string text) { m_text = text; };
    void setCallback(std::function<void()> callback) { m_callback = callback; };

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
    // anvil::TextLabel* m_label = nullptr;

    bool m_released;
};

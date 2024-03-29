#pragma once

#include <functional>
#include "anvil.h"

class MenuButton : public anvil::GameObject
{
public:
    MenuButton();

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;

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
    bool released_;

};

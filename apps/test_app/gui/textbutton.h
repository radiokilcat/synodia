#pragma once

#include <functional>
#include <string>
#include "anvil.h"


class TextButton: public anvil::GameObject
{
public:
    TextButton(const anvil::LoaderParams* params,
               std::string text,
               std::function<void()> callback);

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;

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

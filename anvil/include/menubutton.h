#pragma once

#include "gameobject.h"
#include <functional>


namespace anvil {


class MenuButton : public GameObject
{
public:
    MenuButton(const LoaderParams* params, std::function<void()> callback);

    void draw(std::shared_ptr<Renderer> renderer) override;
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

    bool released_;

};


}

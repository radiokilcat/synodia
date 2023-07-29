#pragma once

#include <SDL3/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "vector2d.h"
#include "renderer.h"
#include "texturemanager.h"
#include "gameobject.h"
#include "inputhandler.h"
#include "fontloader.h"


namespace anvil {

struct Color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

class TextLabel : public BaseGameObject
{
public:
    TextLabel(std::string id, std::string text, Color color, float x, float y, float w, float h,
              TTF_Font* font = FontLoader::instance()->getDefaultFont());
    TextLabel() = default;

    virtual void draw(std::shared_ptr<Renderer> renderer) override;
    virtual void update() override {};
    virtual void clean() override {};
    virtual ~TextLabel() {};

protected:
    std::string id_;
    std::string text_;
    TTF_Font* font_;
    SDL_Color color_;

    Vector2D position_;
    int width_, height_;
};

}

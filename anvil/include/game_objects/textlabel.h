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

class TextLabel : public GameObject
{
public:
    TextLabel(std::string text, Color color, TTF_Font* font = FontLoader::instance()->getDefaultFont());
    TextLabel() = default;

    virtual void draw(std::shared_ptr<Renderer> renderer) override;
    void drawWrapped(std::shared_ptr<Renderer> renderer, Uint32 wrapLength);
    void setPosition(float x, float y);
    void setSize(float w, float h);
    void setText(std::string text);
    virtual void update() override {};
    virtual void clean() override {};
    virtual ~TextLabel() {};

    void setTextScale(int size);

protected:
    std::string text_;
    TTF_Font* font_;
    int textScale_ = 1;
    SDL_Color color_;
};

}

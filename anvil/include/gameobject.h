#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <string>

#include "vector2d.h"
#include "renderer.h"
#include "texturemanager.h"

namespace anvil {

class LoaderParams
{
public:
    LoaderParams(int x, int y, int width, int height, std::string id)
        : x_(x)
        , y_(y)
        , width_(width)
        , height_(height)
        , texture_id_(id)
        {};
    int getX() const          { return x_; }
    int getY() const          { return y_; }
    int getWidth() const      { return width_; }
    int getHeight() const     { return height_; }
    std::string getId() const { return texture_id_; }
private:
    int x_, y_;
    int width_, height_;
    std::string texture_id_;
};

class GameObject
{
public:
    GameObject(const LoaderParams* params)
        : position_(params->getX(), params->getY())
        , velocity_(0,0)
        , acceleration_(0,0)
    {
        width_ = params->getWidth();
        height_ = params->getHeight();
        id_ = params->getId();
        currentRow_ = 1;
        currentFrame_ = 1;
    };
    virtual void draw(std::shared_ptr<Renderer>& renderer)
    {
        TextureManager::instance()->drawFrame(id_, (Uint32)position_.x(), (Uint32)position_.y(),
                                              width_, height_,
                                              currentRow_, currentFrame_,
                                              renderer->getRenderer());
    };
    virtual void update() = 0;
    virtual void clean() = 0;
    virtual ~GameObject() {};

protected:
    std::string id_;
    int currentFrame_;
    int currentRow_;
    Vector2D position_;
    Vector2D velocity_;
    Vector2D acceleration_;
    int width_, height_;
};

}

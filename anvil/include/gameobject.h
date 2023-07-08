#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "json_serializer/serializable_base.h"
#include "json_serializer/json_serializer.h"
#include "vector2d.h"
#include "renderer.h"
#include "texturemanager.h"
#include "inputhandler.h"
#include "application.h"

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

class BaseGameObject
{
public:
    BaseGameObject() {};
    virtual ~BaseGameObject() {};

    virtual void draw(std::shared_ptr<Renderer> renderer) = 0;
    virtual void update() = 0;
    virtual void clean() = 0;
};

class GameObject : public BaseGameObject, public SerializableBase
{
public:
    GameObject() :
        position_(0, 0),
        velocity_(.0f, .0f), 
        acceleration_(0,0),
        id_("") {
        currentFrame_ = 0;
        currentRow_ = 0;
        height_ = 0;
        width_ = 0;
    }

    GameObject(const LoaderParams* params)
        : position_((float)params->getX(), (float)params->getY())
        , velocity_(0,0)
        , acceleration_(0,0)
    {
        width_ = params->getWidth();
        height_ = params->getHeight();
        id_ = params->getId();
        currentRow_ = 1;
        currentFrame_ = 1;
    };
    virtual void draw(std::shared_ptr<Renderer> renderer) override
    {
        TextureManager::instance()->drawFrame(id_, (Uint32)position_.x(), (Uint32)position_.y(),
                                              width_, height_,
                                              currentRow_, currentFrame_,
                                              renderer->getRenderer());
    };
    virtual void update() override {
        velocity_ += acceleration_;
        position_ += velocity_;
    };
    virtual void clean() override {};
    virtual ~GameObject() {};

    void from_json(nlohmann::json &j) override {
        id_ = j["id_"];
        currentFrame_ = j["currentFrame_"];
        currentRow_ = j["currentRow_"];
        position_ = j["position_"];
        velocity_ = j["velocity_"];
        acceleration_ = j["acceleration_"];
        width_ = j["width_"];
        height_ = j["height_"];
    }
    void to_json(nlohmann::json &j) override {
        j["id_"] = id_;
        j["currentFrame_"] = currentFrame_;
        j["currentRow_"] = currentRow_;
        j["position_"] = position_;
        j["velocity_"] = velocity_;
        j["acceleration_"] = acceleration_;
        j["width_"] = width_;
        j["height_"] = height_;
    }

    bool operator==(const GameObject& g) {
        return id_ == g.id_;
    }
protected:
    std::string id_;
    int currentFrame_ = 0;
    int currentRow_ = 0;
    Vector2D position_;
    Vector2D velocity_;
    Vector2D acceleration_;
    int width_, height_;
};

}

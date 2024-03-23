#pragma once

#include "json_serializer/serializable_base.h"
#include "json_serializer/json_serializer.h"
#include "renderer.h"

namespace anvil {

class LoaderParams
{
public:
    LoaderParams(float x, float y, float width, float height, std::string id)
        : x_(x)
        , y_(y)
        , width_(width)
        , height_(height)
        , texture_id_(id)
        {};
    float getX() const          { return x_; }
    float getY() const          { return y_; }
    float getWidth() const      { return width_; }
    float getHeight() const     { return height_; }
    std::string getId() const { return texture_id_; }

private:
    float x_, y_;
    float width_, height_;
    std::string texture_id_;
};

class BaseGameObject: public SerializableBase
{
public:
    BaseGameObject() {};
    virtual ~BaseGameObject() {};

    virtual void draw(std::shared_ptr<Renderer> renderer) = 0;
    virtual void update() = 0;
    virtual void clean() = 0;
    virtual void load(const LoaderParams* params) = 0;
    virtual void init() = 0;
    virtual int  getZOrder() = 0;

    virtual void setCallback(std::function<void()> callback) { };

    virtual void addChild(std::unique_ptr<BaseGameObject> child) = 0;
    virtual void removeChild(std::unique_ptr<BaseGameObject> child) = 0;
    virtual const std::vector<std::unique_ptr<BaseGameObject>>& getChildren() = 0;

    void from_json(nlohmann::json&) override {
    }
    void to_json(nlohmann::json&) override {
    }
};

}

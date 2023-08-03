#pragma once

#include "json_serializer/serializable_base.h"
#include "json_serializer/json_serializer.h"
#include "renderer.h"

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

class BaseGameObject: public SerializableBase
{
public:
    BaseGameObject() {};
    virtual ~BaseGameObject() {};

    virtual void draw(std::shared_ptr<Renderer> renderer) = 0;
    virtual void update() = 0;
    virtual void clean() = 0;
    virtual void load(const LoaderParams* params) = 0;
    virtual void addChildObject(std::unique_ptr<anvil::BaseGameObject> gameObject) = 0;

    virtual void setCallback(std::function<void()> callback) { };

    void from_json(nlohmann::json& j) override {
    }
    void to_json(nlohmann::json& j) override {
    }
};

}

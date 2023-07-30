#pragma once

#include "gameobject.h"

namespace anvil {

class IsoGameObject : public BaseGameObject
{
public:
    IsoGameObject(const LoaderParams* params);
    IsoGameObject() {};
    virtual void draw(std::shared_ptr<Renderer> renderer) override;;

    virtual void update();;
    virtual void clean() override {};
    virtual ~IsoGameObject() {};

    void from_json(nlohmann::json& j) override {
        id_ = j["id_"];
        currentFrame_ = j["currentFrame_"];
        currentRow_ = j["currentRow_"];
        position_ = j["position_"];
        velocity_ = j["velocity_"];
        width_ = j["width_"];
        height_ = j["height_"];
    }
    void to_json(nlohmann::json& j) override {
        j["id_"] = id_;
        j["currentFrame_"] = currentFrame_;
        j["currentRow_"] = currentRow_;
        j["position_"] = position_;
        j["velocity_"] = velocity_;
        j["width_"] = width_;
        j["height_"] = height_;
    }
protected:
    std::pair<float, float> getIsoPosition(float x, float y);;

    std::string id_;
    int currentFrame_;
    int currentRow_;
    Vector2D position_;
    Vector2D velocity_;
    int width_, height_;
};

}

#pragma once

#include "json_serializer/serializable_base.h"
#include "json_serializer/json_serializer.h"
#include "renderer.h"

namespace anvil {
    
class IGameObject
{
public:
    virtual ~IGameObject() {};
    virtual void draw(std::shared_ptr<Renderer> renderer) = 0;
    virtual void update() = 0;
    virtual void clean() = 0;
    virtual void init() = 0;
    virtual int  getZOrder() = 0;
    virtual void addChild(std::unique_ptr<IGameObject> child) = 0;
    virtual void removeChild(std::unique_ptr<IGameObject> child) = 0;
    virtual const std::vector<std::unique_ptr<IGameObject>>& getChildren() = 0;
    virtual void setCallback(std::function<void()> callback) {};
    virtual void from_json(const nlohmann::json&) = 0;
    virtual void to_json(nlohmann::json&) = 0;
};

}

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
#include "BaseGameObject.h"


namespace anvil {

enum class Direction { Up, Down, Left, Right, Static };

class GameObject : public BaseGameObject
{
public:
    GameObject();

    virtual void draw(std::shared_ptr<Renderer> renderer) override;
    virtual void drawScaled(std::shared_ptr<Renderer> renderer, float scale);
    virtual void update() override {};
    virtual void clean() override {};
    virtual ~GameObject() {};
    virtual void load(const LoaderParams* params) override;
    virtual void addChildObject(std::unique_ptr<anvil::BaseGameObject> gameObject);
    virtual int getZOrder();

    void from_json(nlohmann::json &j);
    void to_json(nlohmann::json &j);

    bool operator==(const GameObject& g);

protected:
    std::string id_;
    std::string textureId_;
    int currentFrame_ = 0;
    int currentRow_ = 0;
    Vector2D position_;
    Vector2D velocity_;
    Vector2D acceleration_;
    int width_, height_;
    int zOrder_;

    std::vector<std::unique_ptr<anvil::BaseGameObject>> m_childs;
};

}

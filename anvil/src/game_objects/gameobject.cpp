#pragma once

#include "game_objects/gameobject.h"

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

GameObject::GameObject() :
    position_(0, 0),
    velocity_(.0f, .0f),
    acceleration_(0,0),
    id_("") {
    currentFrame_ = 0;
    currentRow_ = 0;
    height_ = 0;
    width_ = 0;
}

GameObject::GameObject(const LoaderParams* params)
    : position_((float)params->getX(), (float)params->getY())
    , velocity_(0,0)
    , acceleration_(0,0)
{
    width_ = params->getWidth();
    height_ = params->getHeight();
    id_ = params->getId();
    currentRow_ = 1;
    currentFrame_ = 1;
}

void GameObject::draw(std::shared_ptr<Renderer> renderer)
{
    TextureManager::instance()->drawFrame(id_, (Uint32)position_.x(), (Uint32)position_.y(),
                                          width_, height_,
                                          currentRow_, currentFrame_,
                                          renderer->getRenderer());
}

void GameObject::drawScaled(std::shared_ptr<Renderer> renderer, float scale)
{
    TextureManager::instance()->drawFrameScaled(id_, scale,(Uint32)position_.x(), (Uint32)position_.y(),
        width_, height_,
        currentRow_, currentFrame_,
        renderer->getRenderer());
}

void GameObject::from_json(nlohmann::json& j) {
    id_ = j["id_"];
    currentFrame_ = j["currentFrame_"];
    currentRow_ = j["currentRow_"];
    position_ = j["position_"];
    velocity_ = j["velocity_"];
    acceleration_ = j["acceleration_"];
    width_ = j["width_"];
    height_ = j["height_"];
}

void GameObject::to_json(nlohmann::json& j)  {
    j["id_"] = id_;
    j["currentFrame_"] = currentFrame_;
    j["currentRow_"] = currentRow_;
    j["position_"] = position_;
    j["velocity_"] = velocity_;
    j["acceleration_"] = acceleration_;
    j["width_"] = width_;
    j["height_"] = height_;
}

bool GameObject::operator==(const GameObject& g) {
    return id_ == g.id_;
}

}

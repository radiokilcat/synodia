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

void GameObject::load(const LoaderParams* params)
{
    position_ = Vector2D{(float)params->getX(), (float)params->getY()};
    velocity_ = Vector2D{0,0};
    acceleration_ = Vector2D{0,0};
    width_ = params->getWidth();
    height_ = params->getHeight();
    id_ = params->getId();
    currentRow_ = 1;
    currentFrame_ = 1;
}

void GameObject::addChildObject(std::unique_ptr<BaseGameObject> gameObject)
{
    m_childs.push_back(std::move(gameObject));
    // TODO: Uncomment when GameScene::addChildObject fixed
//    std::sort(m_childs.begin(), m_childs.end(), [](const std::unique_ptr<anvil::BaseGameObject>& a,
//                                                   const std::unique_ptr<anvil::BaseGameObject>& b) {
//        return a->getZOrder() < b->getZOrder();
//    });
}

int GameObject::getZOrder()
{
    return zOrder_;
}

void GameObject::from_json(nlohmann::json& j) {
    id_ = j.value("id", std::string("test1"));
    textureId_ = j.value("textureId", std::string("test"));
    currentFrame_ = j.value("currentFrame", 0);
    currentRow_ = j.value("currentRow", 0);
    position_ = j.value("position", Vector2D{0,0});
    velocity_ = j.value("velocity", Vector2D{0,0});
    acceleration_ = j.value("acceleration", Vector2D{0,0});
    width_ = j.value("width", 0);
    height_ = j.value("height", 0);
    zOrder_ = j.value("zOrder", 0);
}

void GameObject::to_json(nlohmann::json& j)  {
    j["id"] = id_;
    j["textureId"] = textureId_;
    j["currentFrame"] = currentFrame_;
    j["currentRow"] = currentRow_;
    j["position"] = position_;
    j["velocity"] = velocity_;
    j["acceleration"] = acceleration_;
    j["width"] = width_;
    j["height"] = height_;
    j["zOorder"] = zOrder_;
}

bool GameObject::operator==(const GameObject& g) {
    return id_ == g.id_;
}
}

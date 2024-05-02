#pragma once

#include "game_objects/gameobject.h"

#include <string>
#include <nlohmann/json.hpp>
#include "renderer.h"
#include "logger.h"
#include <fmt/format.h>

#include "components/IComponent.h"
#include "components/Sprite2DComponent.h"
#include "components/Transform2DComponent.h"
#include "components/Collision2DComponent.h"

namespace anvil {

void GameObject::draw(std::shared_ptr<Renderer> renderer)
{
    auto transform = getComponent<Transform2DComponent>();
    auto sprite = getComponent<Sprite2DComponent>();
    auto x = transform->getX();
    auto y = transform->getY();
        
    sprite->draw(x, y, renderer);
    for (const auto& child: m_children ) {
        child->draw(renderer);
    }
}

void GameObject::drawScaled(std::shared_ptr<Renderer> renderer, float scale)
{
    auto transform = getComponent<Transform2DComponent>();
    auto sprite = getComponent<Sprite2DComponent>();
    auto x = transform->getX();
    auto y = transform->getY();
    sprite->drawScaled(x, y, renderer);
}

void GameObject::load() {
}

void GameObject::update(Uint64 deltaTime) {
    for (const auto& component: components_ ) {
        component.second->update(deltaTime);
    }
    for (const auto& child: m_children ) {
        child->update(deltaTime);
    }
}


int GameObject::getLayer() {
    return layer_;
}

void GameObject::baseInit()
{
    // auto debugLabel = std::make_shared<TextLabel>(id_, anvil::Color{255, 0, 0});
    // debugLabel->setPosition(position_.x(), position_.y() - 15.f);
    // debugLabel->setSize(width_, 15.f);
    // addChild(debugLabel);
    this->init();
    sort_children();
    for (const auto& child: m_children ) {
        child->baseInit();
    }
}

void GameObject::sort_children() {
    std::sort(m_children.begin(), m_children.end(), [](const std::shared_ptr<anvil::IGameObject>& a,
                                               const std::shared_ptr<anvil::IGameObject>& b) {
    return a->getLayer() < b->getLayer();
    });
}

void GameObject::from_json(const nlohmann::json& j) {
    id_ = j.value("id", std::string("default_id"));
    layer_ = j.value("layer", 0);
    
    for (const auto& component: components_) {
        component.second->from_json(j);
    }
}

void GameObject::to_json(nlohmann::json& j)  {
    j["id"] = id_;
    j["layer"] = layer_;
    
    for (const auto& component: components_) {
        component.second->to_json(j);
    }
}

bool GameObject::isCollide(std::shared_ptr<IGameObject> other) {
    auto collider = getComponent<CollisionComponent>();
    auto otherCollider = other->getComponent<CollisionComponent>();
    if (!collider || !otherCollider) {
        return false;
    }
    return collider->checkCollision(*otherCollider);
}
}

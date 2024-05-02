#include "stationary.h"

#include "components/Collision2DComponent.h"
#include "components/Sprite2DComponent.h"
#include "components/Transform2DComponent.h"

Stationary::Stationary() {

}

void Stationary::draw(std::shared_ptr<anvil::Renderer> renderer) {
    auto x = transform_->getX();
    auto y = transform_->getY();
    sprite_->draw(x, y, renderer);
    if (outline_) {
        auto collider = getComponent<anvil::CollisionComponent>();
        collider->drawOutline(renderer);
    }
}

void Stationary::from_json(const nlohmann::json& j) {
    GameObject::from_json(j);
}

void Stationary::to_json(nlohmann::json& j) {
    GameObject::to_json(j);
}

void Stationary::update(Uint64 deltaTime) {
    // for (auto component: components_) {
    //     component.second->update(deltaTime);
    // }
    transform_->update(deltaTime);
    auto collider = getComponent<anvil::CollisionComponent>();
    collider->update(deltaTime);
}

void Stationary::clean() {
}

void Stationary::init() {
    if (!sprite_) {
        sprite_ = getComponent<anvil::Sprite2DComponent>();
    }
    if (!transform_) {
        transform_ = getComponent<anvil::Transform2DComponent>();
    }
}

bool Stationary::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("Stationary", []() -> std::unique_ptr<anvil::IGameObject> {
        return std::make_unique<Stationary>();
    });
    return true;
}
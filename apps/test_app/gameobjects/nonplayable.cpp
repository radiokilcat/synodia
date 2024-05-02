#include "nonplayable.h"

#include "components/Collision2DComponent.h"
#include "components/MovementIsoComponent.h"
#include "components/Sprite2DComponent.h"

NonPlayable::NonPlayable()
{
}

void NonPlayable::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    auto x = transform_->getX();
    auto y = transform_->getY();
    sprite_->draw(x, y, renderer);
    if (outline_) {
        auto collider = getComponent<anvil::CollisionComponent>();
        collider->drawOutline(renderer);
    }
}

void NonPlayable::update(Uint64 deltaTime)
{
    auto direction = movement_->moveDirection();
    switch (direction)
    {
    case anvil::Direction::Up:
        sprite_->setCurrentRow(5);
        break;
    case anvil::Direction::Down:
        sprite_->setCurrentRow(11);
        break;
    case anvil::Direction::Left:
        sprite_->setCurrentRow(10);
        break;
    case anvil::Direction::Right:
        sprite_->setCurrentRow(12);
        break;
    case anvil::Direction::Static:
        sprite_->setCurrentRow(3);
        break;
    }
    
    sprite_->setCurrentRow(10);
    // movement_->setVelocity(anvil::Vector2D(0, 3.1f));
    
    sprite_->setCurrentFrame(int((anvil::Application::Instance()->getTicks() / 100) % 6));
    
    // if (newPosition.x() + width_ >= anvil::Application::Instance()->getScreenWidth()
    //     || newPosition.x() < 0) {
    //     velocity_.setX(-1 * velocity_.x());
    //     velocity_.setY(-1 * velocity_.y());
    // }
    // if (newPosition.y() + height_ >= anvil::Application::Instance()->getScreenHeight()
    //     || newPosition.y() < 0) {
    //     velocity_.setX(-1 * velocity_.x());
    //     velocity_.setY(-1 * velocity_.y());
    // }
    for (auto component: components_) {
        component.second->update(deltaTime);
    }
}

void NonPlayable::init() {
    if (!sprite_) {
        sprite_ = getComponent<anvil::Sprite2DComponent>();
    }
    if (!transform_) {
        transform_ = getComponent<anvil::Transform2DComponent>();
    }
    if (!movement_) {
        movement_ = getComponent<anvil::MovementIsoComponent>();
    }
    sprite_->setCurrentFrame(0);
    sprite_->setCurrentRow(1);
}

void NonPlayable::clean()
{
}

void NonPlayable::from_json(const nlohmann::json& j)
{
    GameObject::from_json(j);
}

void NonPlayable::to_json(nlohmann::json& j)
{
    GameObject::to_json(j);

}

bool NonPlayable::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("NonPlayable", []() -> std::unique_ptr<anvil::IGameObject> {
        return std::make_unique<NonPlayable>();
    });
    return true;
}


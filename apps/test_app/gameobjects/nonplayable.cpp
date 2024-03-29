#include "nonplayable.h"

NonPlayable::NonPlayable()
: anvil::IsoGameObject()
{

}

void NonPlayable::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::IsoGameObject::draw(renderer);
}

void NonPlayable::update()
{
    auto direction = moveDirection();
    switch (direction)
    {
    case anvil::Direction::Up:
        currentRow_ = 5;
        break;
    case anvil::Direction::Down:
        currentRow_ = 11;
        break;
    case anvil::Direction::Left:
        currentRow_ = 10;
        break;
    case anvil::Direction::Right:
        currentRow_ = 12;
        break;
    case anvil::Direction::Static:
        currentRow_ = 3;
        break;
    }

    currentFrame_ = int((anvil::Application::Instance()->getTicks() / 100) % 6);
    auto newPosition = position_ + velocity_;

    if (newPosition.x() + width_ >= anvil::Application::Instance()->getScreenWidth()
        || newPosition.x() < 0) {
        velocity_.setX(-1 * velocity_.x());
        velocity_.setY(-1 * velocity_.y());
    }
    if (newPosition.y() + height_ >= anvil::Application::Instance()->getScreenHeight()
        || newPosition.y() < 0) {
        velocity_.setX(-1 * velocity_.x());
        velocity_.setY(-1 * velocity_.y());
    }
    anvil::IsoGameObject::update();
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
    anvil::GameObjectFactory::instance().registerType("NonPlayable", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<NonPlayable>();
    });
    return true;
}


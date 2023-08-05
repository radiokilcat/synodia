#include "nonplayable.h"

NonPlayable::NonPlayable()
: anvil::IsoGameObject()
{

}

void NonPlayable::draw(std::shared_ptr<anvil::Renderer> renderer)
{
//    anvil::GameObject::draw(renderer);
    anvil::IsoGameObject::draw(renderer);

}

void NonPlayable::update()
{
    currentRow_ = 3;
    currentFrame_ = int((anvil::Application::Instance()->getTicks() / 100) % 6);
    auto newPostion = position_ + velocity_;
    auto newPositionIso = getIsoPosition(newPostion.x(), newPostion.y());

    if (newPositionIso.first + width_ >= anvil::Application::Instance()->getScreenWidth()
        || newPositionIso.first < 0) {
        velocity_.setX(-1 * velocity_.x());
        velocity_.setY(-1 * velocity_.y());
    }
    if (newPositionIso.second + height_ >= anvil::Application::Instance()->getScreenHeight()
        || newPositionIso.second < 0) {
        velocity_.setX(-1 * velocity_.x());
        velocity_.setY(-1 * velocity_.y());
    }
    anvil::IsoGameObject::update();
}

void NonPlayable::clean()
{
}

void NonPlayable::from_json(nlohmann::json& j)
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


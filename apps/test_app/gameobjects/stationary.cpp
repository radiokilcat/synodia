#include "stationary.h"

Stationary::Stationary()
: anvil::IsoGameObject()
{

}

void Stationary::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    auto isoPos = getIsoPosition(position_.x(), position_.y());
    anvil::TextureManager::instance()->draw(textureId_, isoPos.first, isoPos.second, width_, height_,
                                            renderer->getRenderer());
}

void Stationary::from_json(nlohmann::json& j)
{
    GameObject::from_json(j);
}

void Stationary::to_json(nlohmann::json& j)
{
    GameObject::to_json(j);
}

void Stationary::update()
{
}

void Stationary::clean()
{
}

bool Stationary::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("Stationary", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<Stationary>();
    });
    return true;
}


#include "nonplayable.h"

NonPlayable::NonPlayable()
: anvil::GameObject()
{

}

void NonPlayable::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::GameObject::draw(renderer);
}

void NonPlayable::update()
{
}

void NonPlayable::clean()
{
}

bool NonPlayable::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("NPC", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<NonPlayable>();
    });
    return true;
}


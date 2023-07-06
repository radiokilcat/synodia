#include "nonplayable.h"

NonPlayable::NonPlayable(const anvil::LoaderParams* params)
    : anvil::GameObject(params)
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


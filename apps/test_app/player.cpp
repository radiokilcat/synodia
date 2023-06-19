#include "player.h"


Player::Player(const anvil::LoaderParams* params)
    : anvil::GameObject(params)
{

}

void Player::draw(std::shared_ptr<anvil::Renderer>& renderer)
{
    anvil::GameObject::draw(renderer);
}

void Player::update()
{
}

void Player::clean()
{
}

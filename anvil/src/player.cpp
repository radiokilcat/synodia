#include "player.h"

namespace anvil {

Player::Player(const LoaderParams* params)
    : GameObject(params)
{

}

void Player::draw(std::shared_ptr<Renderer>& renderer)
{
    GameObject::draw(renderer);
}

void Player::update()
{
}

void Player::clean()
{

}

}

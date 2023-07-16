#include "player.h"


Player::Player(const anvil::LoaderParams* params)
    : anvil::IsoGameObject(params)
{

}

void Player::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::IsoGameObject::draw(renderer);
}

void Player::update()
{
    velocity_.setX(0.f);
    velocity_.setY(0.f);
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Right))
    {
        velocity_.setX(0.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Left))
    {
        velocity_.setX(-0.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Up))
    {
        velocity_.setY(-0.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Down))
    {
        velocity_.setY(0.1f);
    }
    // Cycle through the spreadsheet and change frame position
    currentFrame_ = int((anvil::Application::Instance()->getTicks() / 100) % 6);
    anvil::IsoGameObject::update();
}

void Player::clean()
{
}

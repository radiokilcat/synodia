#include "player.h"
#include <ctime>
#include <cstdlib>

Player::Player()
{
    srand(time(NULL));
    _speech = std::make_unique<Speech>();
    _speech->load(new anvil::LoaderParams(15, 15, 62, 40, "speech"));
}

void Player::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::IsoGameObject::draw(renderer);
    _speech->draw(renderer);
}

void Player::update()
{
    velocity_.setX(0.f);
    velocity_.setY(0.f);
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Right))
    {
        velocity_.setX(3.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Left))
    {
        velocity_.setX(-3.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Up))
    {
        velocity_.setY(-3.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Down))
    {
        velocity_.setY(3.1f);
    }
    if (anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Space))
    {
        if (!_speech->isShown())
        {
            auto ind = rand() % std::size(_quotes);
            auto text = _quotes[ind];
            _speech->show(5, text);
        }
    }
    // Cycle through the spreadsheet and change frame position
    currentFrame_ = int((anvil::Application::Instance()->getTicks() / 100) % 6);
    anvil::IsoGameObject::update();
    auto pos = getIsoPosition(position_.x(), position_.y());
    _speech->setPosition(pos.first - 80, pos.second - 90);
    _speech->update();
}

void Player::clean()
{
}

bool Player::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("Player", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<Player>();
    });
    return true;
}

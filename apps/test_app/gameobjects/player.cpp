#include <iostream>
#include "player.h"
#include <ctime>
#include <cstdlib>

Player::Player()
{
    srand(time(NULL));
    _speech = std::make_unique<Speech>();
    _speech->load(new anvil::LoaderParams(15, 15, 62, 40, "speech"));
}

int Player::getX()
{
    return IsoGameObject::getX();
}

int Player::getY()
{
    return IsoGameObject::getY();
}

int Player::getWidth()
{
    return width_;
}

int Player::getHeight()
{
    return height_;
}

void Player::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    anvil::IsoGameObject::draw(renderer);
    _speech->draw(renderer);
}

void Player::update()
{
    currentRow_ = 12;
    if (!_continuedMove)
    {
        setVelocity(0, 0);
    }
    if (!_blockInteractions)
    {
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Right))
        {
            setVelocity(3.1f, 0);
        }
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Left))
        {
            currentRow_ = 10;
            setVelocity(-3.1f, 0);
        }
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Up))
        {
            currentRow_ = 12;
            setVelocity(0, -3.1f);
        }
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Down))
        {
            currentRow_ = 10;
            setVelocity(0, 3.1f);
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
    }
    else {
            if (!_speech->isShown())
            {
                auto text = "I found it!\nThe Cultists Scrolls!";
                _speech->show(3, text);
                _speech->setTextSize(2.0f);
                _speech->setSize(70.f, 50.f);
            }
    }
    currentFrame_ = int((anvil::Application::Instance()->getTicks() / 100) % 6);
    auto newPosition = position_ + velocity_;

    if (newPosition.x() + width_ >= anvil::Application::Instance()->getScreenWidth()
        || newPosition.x() < 0) {
        setVelocity(-1 * velocity_.x(), -1 * velocity_.y());
    }
    if (newPosition.y() + height_ >= anvil::Application::Instance()->getScreenHeight()
        || newPosition.y() < 0) {
        setVelocity(-1 * velocity_.x(), -1 * velocity_.y());
    }
    if (_inverseMove) {
        setVelocity(-1 * velocity_.x(), -1 * velocity_.y());
        _inverseMove = false;
    }
    anvil::IsoGameObject::update();
//    auto pos = getIsoPosition(position_.x(), position_.y());
    _speech->setPosition(position_.x() - 80, position_.y() - 90);
    _speech->update();
}

void Player::clean()
{
}

void Player::from_json(nlohmann::json& j)
{
    GameObject::from_json(j);
}

void Player::to_json(nlohmann::json& j)
{
    GameObject::to_json(j);
}

bool Player::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("Player", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<Player>();
    });
    return true;
}

bool Player::continuedMove() const
{
    return _continuedMove;
}

void Player::setContinuedMove(bool isContinuedMove)
{
    _continuedMove = isContinuedMove;
}

void Player::setInverseMove(bool isInverseMove)
{
    _inverseMove = isInverseMove;
    if (isInverseMove) {
        velocity_.setX(-1 * velocity_.x());
        velocity_.setY(-1 * velocity_.y());
        anvil::IsoGameObject::update();
    }

}

void Player::blockInteractions()
{
    _blockInteractions = true;
}

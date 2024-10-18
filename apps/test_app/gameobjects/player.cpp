#include <iostream>
#include "player.h"
#include <ctime>
#include <cstdlib>

#include "components/Collision2DComponent.h"
#include "components/MovementIsoComponent.h"
#include "components/Sprite2DComponent.h"

namespace anvil {
    class Sprite2DComponent;
}

Player::Player()
{
    // srand(time(NULL));
    // _speech = std::make_unique<Speech>();
    // _speech->load(new anvil::GameObjectData(15, 15, 62, 40, "speech"));
}

void Player::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    // anvil::IsoGameObject::draw(renderer);
    // _speech->draw(renderer);
    sprite_->draw_sheet(renderer);
    if (outline_) {
        auto collider = getComponent<anvil::CollisionComponent>();
        collider->drawOutline(renderer);
        
    }
}

void Player::update(Uint64 deltaTime)
{
     if (!_continuedMove) {
         movement_->setVelocity(anvil::Vector2D(0, 0));
     }
    if (!_blockInteractions)
    {
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Right)) {
            // sprite_->setCurrentSheet("lady_run_right");
            sprite_->setCurrentAnimation("run", anvil::Direction::Right);
            lastDirection_ = anvil::Direction::Right;
            movement_->setVelocity(anvil::Vector2D(2.5f, 0));
        }
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Left)) {
            // sprite_->setCurrentSheet("lady_run_left");
            sprite_->setCurrentAnimation("run", anvil::Direction::Left);
            lastDirection_ = anvil::Direction::Left;
            movement_->setVelocity(anvil::Vector2D(-2.5f, 0));
        }
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Up)) {
            // sprite_->setCurrentSheet("lady_run_top");
            sprite_->setCurrentAnimation("run", anvil::Direction::Up);
            lastDirection_ = anvil::Direction::Up;
            movement_->setVelocity(anvil::Vector2D(0, 2.5f));
        }
        if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Down)) {
            // sprite_->setCurrentSheet("lady_run_bottom");
            // sprite_->setCurrentRow(1);
            sprite_->setCurrentAnimation("run", anvil::Direction::Down);
            lastDirection_ = anvil::Direction::Down;
            movement_->setVelocity(anvil::Vector2D(0, -2.5f));
        }
        if (anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Space))
        {
            sprite_->setCurrentAnimation("attack", lastDirection_);
            // if (!_speech->isShown())
            // {
            //     auto ind = rand() % std::size(_quotes);
            //     auto text = _quotes[ind];
            //     _speech->show(5, text);
            // }
        }
        if (anvil::InputHandler::instance()->isAnyKeyUp()) {
            if (movement_->getVelocity().isZero())
                sprite_->setCurrentAnimation("idle", lastDirection_);
        }
    }
    // else {
    //         if (!_speech->isShown())
    //         {
    //             auto text = "I found it!\nThe Cultists Scrolls!";
    //             _speech->show(3, text);
    //             _speech->setTextSize(2.0f);
    //             _speech->setSize(70.f, 50.f);
    //         }
    // }
    // sprite_->setFrameAndRow(int((anvil::Application::Instance()->getTicks() / 100) % 4));

    // if (newPosition.x() + width_ >= anvil::Application::Instance()->getScreenWidth()
    //     || newPosition.x() < 0) {
    //     setVelocity(-1 * velocity_.x(), -1 * velocity_.y());
    // }
    // if (newPosition.y() + height_ >= anvil::Application::Instance()->getScreenHeight()
    //     || newPosition.y() < 0) {
    //     setVelocity(-1 * velocity_.x(), -1 * velocity_.y());
    // }
    if (_inverseMove) {
        auto velocity = movement_->getVelocity();
        movement_->setVelocity(anvil::Vector2D(-1 * movement_->getVelocity().x(), -1 * velocity.y()));
        _inverseMove = false;
    }
//    auto pos = getIsoPosition(position_.x(), position_.y());
    // _speech->setPosition(transform_->getX() - 80, transform_->getY() - 90);
    // _speech->update();
    for (auto component: components_) {
        component.second->update(deltaTime);
    }
}

void Player::clean()
{
}

void Player::init() {
    if (!sprite_) {
        sprite_ = getComponent<anvil::Sprite2DComponent>();
    }
    if (!transform_) {
        transform_ = getComponent<anvil::Transform2DComponent>();
    }
    if (!movement_) {
        movement_ = getComponent<anvil::MovementIsoComponent>();
    }
    sprite_->setCurrentAnimation("idle", anvil::Direction::Down);
    sprite_->setCurrentFrame(0);
    sprite_->setCurrentRow(1);
}

void Player::from_json(const nlohmann::json& j)
{
    GameObject::from_json(j);
}

void Player::to_json(nlohmann::json& j)
{
    GameObject::to_json(j);
}

bool Player::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("Player", []() -> std::unique_ptr<anvil::IGameObject> {
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
    // _inverseMove = isInverseMove;
    // if (isInverseMove) {
    //     velocity_.setX(-1 * velocity_.x());
    //     velocity_.setY(-1 * velocity_.y());
    //     anvil::IsoGameObject::update();
    // }
}

void Player::blockInteractions()
{
    _blockInteractions = true;
}

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
    auto [isoX, isoY] = getIsoPosition(position_.x(), position_.y());
    return isoX;
}

int Player::getY()
{
    auto [isoX, isoY] = getIsoPosition(position_.x(), position_.y());
    return isoY;
}

//int Player::getScreenX()
//{
//    return position;
//}

//int Player::getScreenY()
//{
//    auto [isoX, isoY] = getIsoPosition(position_.x(), position_.y());
//    return isoY;
//}
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
    int x1 = getX();
    int y1 = getY();

    int x2 = x1 + width_;
    int y2 = y1;

    int x3 = x1 + width_;
    int y3 = y1 + height_;

    int x4 = x1;
    int y4 = y1 + height_;

    /*anvil::TextureManager::instance()->drawQuadrilateral(renderer->getRenderer(), x1, y1,
                                                         x2, y2,
                                                         x3, y3,
                                                       x4, y4); */ 
}

void Player::update()
{
    currentRow_ = 12;
    velocity_.setX(0.f);
    velocity_.setY(0.f);
    if (!_continuedMove)
    {
        velocity_.setX(0.f);
        velocity_.setY(0.f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Right))
    {
        velocity_.setX(3.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Left))
    {
        currentRow_ = 10;
        velocity_.setX(-3.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Up))
    {
        currentRow_ = 12;
        velocity_.setY(-3.1f);
    }
    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Down))
    {
        currentRow_ = 10;
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
//        currentRow_ = 12;
//        velocity_.setX(0.1f);
    }
//    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Left))
//    {
////        currentRow_ = 10;
////        velocity_.setX(-0.1f);
//    }
//    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Up))
//    {
//        currentRow_ = 12;
//        velocity_.setY(-0.1f);
//    }
//    if(anvil::InputHandler::instance()->isKeyDown(anvil::AnvilKeyCode::Down))
//    {
//        currentRow_ = 10;
//        velocity_.setY(0.1f);
//    }
    // Cycle through the spreadsheet and change frame position
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
    if (_inverseMove) {
        velocity_.setX(-1 * velocity_.x());
        velocity_.setY(-1 * velocity_.y());
        _inverseMove = false;
    }
    anvil::IsoGameObject::update();
    auto pos = getIsoPosition(position_.x(), position_.y());
    _speech->setPosition(pos.first - 80, pos.second - 90);
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

anvil::Direction Player::moveDirection()
{
    if (velocity_.x() > 0 && velocity_.y() == 0)
    {
        return anvil::Direction::Right;
    }
    if (velocity_.y() > 0 && velocity_.x() == 0)
    {
        return anvil::Direction::Down;
    }
    if (velocity_.y() < 0 && velocity_.x() == 0)
    {
        return anvil::Direction::Up;
    }
    if (velocity_.y() == 0 && velocity_.x() == 0)
    {
        return anvil::Direction::Static;
    }
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
#include "game_objects/isogameobject.h"
#include "game_objects/textlabel.h"

namespace anvil {

bool IsoGameObject::isNearlyZero(float value, float epsilon)
{
    return std::fabs(value) < epsilon;
}

void IsoGameObject::draw(std::shared_ptr<Renderer> renderer)
{
    TextureManager::instance()->drawFrame(textureId_, (Uint32)position_.x(), (Uint32)position_.y(),
                                                  width_, height_,
                                                  currentRow_, currentFrame_,
                                                  renderer->getRenderer());
    if (debug)
    {
        int x1 = getX();
        int y1 = getY();

        int x2 = x1 + width_;
        int y2 = y1;

        int x3 = x1 + width_;
        int y3 = y1 + height_;

        int x4 = x1;
        int y4 = y1 + height_;

        anvil::TextureManager::instance()->drawQuadrilateral(renderer->getRenderer(), x1, y1,
                                                             x2, y2,
                                                             x3, y3,
                                                           x4, y4);
        for (auto& child: m_childs)
        {
            if(auto label = dynamic_cast<TextLabel*>(child.get()))
            {
                label->draw(renderer);
            }

        }
    }

}

int IsoGameObject::getX()
{
    return position_.x();
}

int IsoGameObject::getY()
{
    return position_.y();
}

void IsoGameObject::setVelocity(float x, float y)
{
    float isoVelocityX = x - y;
    float isoVelocityY = (x + y) / 2;

    velocity_.setX(isoVelocityX);
    velocity_.setY(isoVelocityY);
}

void IsoGameObject::IsoGameObject::moveTowards(const Vector2D& targetPosition, float speed)
{
    Vector2D direction = targetPosition - position_;
    direction.normalize();

    float isoVelocityX = direction.x() - direction.y();
    float isoVelocityY = (direction.x() + direction.y()) / 2;

    isoVelocityX *= speed;
    isoVelocityY *= speed;

    velocity_.setX(isoVelocityX);
    velocity_.setY(isoVelocityY);
}

Direction IsoGameObject::moveDirection()
{
    if (velocity_.x() > 0 && velocity_.y() > 0)
    {
        return anvil::Direction::Down;
    }
    if (velocity_.x() < 0 && velocity_.y() > 0)
    {
        return anvil::Direction::Left;
    }
    if (velocity_.x() > 0 && velocity_.y() < 0)
    {
        return anvil::Direction::Right;
    }
    if (velocity_.x() < 0 && velocity_.y() < 0)
    {
        return anvil::Direction::Up;
    }
    if (isNearlyZero(velocity_.x()) && isNearlyZero(velocity_.x()))
    {
        return anvil::Direction::Static;
    }
}

void IsoGameObject::update(){
    position_ += velocity_;
}

std::pair<float, float> IsoGameObject::getIsoPosition(float x, float y)
{
    float isoX = x - y;
    float isoY = (x + y) / 2;
    return {isoX, isoY};
}

std::pair<float, float> IsoGameObject::getScreenPosition(float isoX, float isoY)
{
    float x = isoY + isoX / 2;
    float y = isoY - isoX / 2;
    return {x, y};
}

bool IsoGameObject::isIntersect(int x, int y, int w, int h) {
    int fxl = getX();
    int fxr = getX() + width_;
    int fyb = getY() + height_;
    int fyt = getY();

    int sxl = x;
    int sxr = x + w;
    int syb = y + h;
    int syt = y;

    if (fxr < sxl || sxr < fxl)
        return false; // No horizontal overlap

    if (fyb < syt || syb < fyt)
        return false; // No vertical overlap
    return true;
}

}

#include "game_objects/isogameobject.h"

namespace anvil {

void IsoGameObject::draw(std::shared_ptr<Renderer> renderer)
{
TextureManager::instance()->drawFrame(textureId_, (Uint32)position_.x(), (Uint32)position_.y(),
                                              width_, height_,
                                              currentRow_, currentFrame_,
                                              renderer->getRenderer());
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

}

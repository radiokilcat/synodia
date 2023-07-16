#include "isogameobject.h"

namespace anvil {

IsoGameObject::IsoGameObject(const LoaderParams* params)
    : position_((float)params->getX(), (float)params->getY())
    , velocity_(0,0)
{
    width_ = params->getWidth();
    height_ = params->getHeight();
    id_ = params->getId();
    currentRow_ = 1;
    currentFrame_ = 1;
}

void IsoGameObject::draw(std::shared_ptr<Renderer> renderer)
{
    std::pair<float, float> isoPosition = getIsoPosition(position_.x(), position_.y());

    TextureManager::instance()->drawFrame(id_, (Uint32)isoPosition.first, (Uint32)isoPosition.second,
                                          width_, height_,
                                          currentRow_, currentFrame_,
                                          renderer->getRenderer());
}

void IsoGameObject::update(){
    position_ += velocity_;
}

std::pair<float, float> IsoGameObject::getIsoPosition(float x, float y)
{
    float isoX = x - y;
    float isoY = (x + y) / 2;
    return {isoX, isoY};
};


}

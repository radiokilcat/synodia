#include "game_objects/isogameobject.h"

namespace anvil {

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

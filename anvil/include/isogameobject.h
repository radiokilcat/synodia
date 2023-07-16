#pragma once

#include "gameobject.h"

namespace anvil {

class IsoGameObject : public BaseGameObject
{
public:
    IsoGameObject(const LoaderParams* params);

    virtual void draw(std::shared_ptr<Renderer> renderer) override;;

    virtual void update();;
    virtual void clean() override {};
    virtual ~IsoGameObject() {};


protected:
    std::pair<float, float> getIsoPosition(float x, float y);;

    std::string id_;
    int currentFrame_;
    int currentRow_;
    Vector2D position_;
    Vector2D velocity_;
    int width_, height_;
};

}

#pragma once

#include "gameobject.h"

namespace anvil {

class IsoGameObject : public GameObject
{
public:
    IsoGameObject() {};
    virtual void draw(std::shared_ptr<Renderer> renderer) override;;
    virtual void update() override;
    virtual ~IsoGameObject() {};

protected:
    std::pair<float, float> getIsoPosition(float x, float y);;
};

}

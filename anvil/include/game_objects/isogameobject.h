#pragma once

#include "gameobject.h"

namespace anvil {

class IsoGameObject : public GameObject
{
public:
    IsoGameObject() {};
    virtual void draw(std::shared_ptr<Renderer> renderer) override;
    virtual void update() override;
    virtual ~IsoGameObject() {};

    std::pair<float, float> getScreenPosition(float isoX, float isoY);
    int getY();
    int getX();

    void setVelocity(float x, float y);
protected:
    std::pair<float, float> getIsoPosition(float x, float y);;
};

}

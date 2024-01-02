#pragma once

#include "gameobject.h"

namespace anvil {

class IsoGameObject : public GameObject
{
public:
    IsoGameObject() {};
    void draw(std::shared_ptr<Renderer> renderer) override;
    void update() override;
    virtual ~IsoGameObject() {};

    std::pair<float, float> getScreenPosition(float isoX, float isoY);
    float getY();
    float getX();

    void setVelocity(float x, float y);
    void moveTowards(const Vector2D& targetPosition, float speed);
    bool isIntersect(float x, float y, float w, float h);
    Direction moveDirection();

protected:
    bool isNearlyZero(float value, float epsilon = 1e-5f);
    std::pair<float, float> getIsoPosition(float x, float y);;
    bool debug = false;
};

}

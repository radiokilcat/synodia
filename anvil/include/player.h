#pragma once

#include "gameobject.h"

namespace anvil {


class Player: public GameObject
{
public:
    Player(const LoaderParams* params);
    ~Player() {};
    void draw(std::shared_ptr<Renderer>& renderer) override;
    void update() override;
    void clean() override;
private:


};


}

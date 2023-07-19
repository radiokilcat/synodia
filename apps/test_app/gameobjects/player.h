#pragma once

#include "anvil.h"


class Player: public anvil::IsoGameObject
{
public:
    Player(const anvil::LoaderParams* params);
    Player() {}
    ~Player() {};
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;
private:


};



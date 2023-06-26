#pragma once

#include "anvil.h"


class Player: public anvil::GameObject
{
public:
    Player(const anvil::LoaderParams* params);
    ~Player() {};
    void draw(std::shared_ptr<anvil::Renderer>& renderer) override;
    void update(const anvil::GameTime deltaTime) override;
    void clean() override;
private:


};



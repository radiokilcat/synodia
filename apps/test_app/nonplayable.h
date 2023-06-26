#pragma once

#include "anvil.h"

class NonPlayable: public anvil::GameObject
{
public:
    NonPlayable(const anvil::LoaderParams* params);
    ~NonPlayable() {};
    void draw(std::shared_ptr<anvil::Renderer>& renderer) override;
    void update(anvil::GameTime deltaTime) override;
    void clean() override;
private:


};

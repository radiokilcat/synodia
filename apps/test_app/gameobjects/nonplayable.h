#pragma once

#include "anvil.h"

namespace anvil {
    class MovementIsoComponent;
    class Sprite2DComponent;
}

class NonPlayable: public anvil::GameObject
{
public:
    NonPlayable();
    ~NonPlayable() {};
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update(Uint64 deltaTime) override;
    void init();
    void clean() override;

    void from_json(const nlohmann::json& j) override;
    void to_json(nlohmann::json& j) override;

    static bool registerWithFactory();
    bool chasing() { return chasing_; }
    void setChaising(bool chasing) { chasing_ = chasing; }

private:
    std::shared_ptr<anvil::Sprite2DComponent> sprite_;
    std::shared_ptr<anvil::Transform2DComponent> transform_;
    std::shared_ptr<anvil::MovementIsoComponent> movement_;
    bool chasing_ = false;
};

#pragma once

#include "anvil.h"

class Stationary: public anvil::GameObject
{
public:
    Stationary();
    ~Stationary() {};
    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update(Uint64 deltaTime) override;
    void clean() override;
    void init() override;

    static bool registerWithFactory();

    void from_json(const nlohmann::json& j);
    void to_json(nlohmann::json& j);
private:
    bool debug = false;
    std::shared_ptr<anvil::Sprite2DComponent> sprite_;
    std::shared_ptr<anvil::Transform2DComponent> transform_;
};

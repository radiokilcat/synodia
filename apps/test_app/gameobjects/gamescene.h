#pragma once

#include "anvil.h"
#include "player.h"

class GameScene : public anvil::GameObject
{
public:
    GameScene() {};
    ~GameScene() {};

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;

    // for now we can load gameScene via load / from_json
    // TODO: probably need to leave one way
    void load(const anvil::LoaderParams* params) override;

    void to_json(nlohmann::json& j) override;
    void from_json(nlohmann::json& j) override;

    static bool registerWithFactory();
};

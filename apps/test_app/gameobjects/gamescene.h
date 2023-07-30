#pragma once

#include "anvil.h"
#include "player.h"

class GameScene : public anvil::BaseGameObject
{
public:
    GameScene() {};
    ~GameScene() {};

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;
    void load(const anvil::LoaderParams* params) override;

    void addGameObject(std::unique_ptr<anvil::BaseGameObject> gameObject);

    void to_json(nlohmann::json& j) override;
    void from_json(nlohmann::json& j) override;

    static bool registerWithFactory();
private:
    std::vector<std::unique_ptr<anvil::BaseGameObject>> m_childs;
};

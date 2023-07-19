#pragma once

#include "anvil.h"
#include "player.h"
class GameScene : public anvil::BaseGameObject
{
public:
    GameScene() {};
    ~GameScene() {};

    void draw(std::shared_ptr<anvil::Renderer> renderer);
    void update();
    void clean();

    void addGameObject(std::unique_ptr<anvil::BaseGameObject> gameObject);

    void to_json(nlohmann::json& j) override;
    void from_json(nlohmann::json& j) override;
private:
    std::vector<std::unique_ptr<anvil::BaseGameObject>> m_childs;
};

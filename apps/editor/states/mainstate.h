#pragma once

#include "gamestate.h"

class GameScene;

class MainState : public anvil::GameState, public SerializableBase
{
public:
    MainState();
    bool onEnter() override;
    bool onExit() override;

    void update(Uint64 deltaTime) override;
    void render() override;

    std::string getID() override;

    void to_json(nlohmann::json& j) override;
    void from_json(const nlohmann::json& j) override;

private:
    std::shared_ptr<anvil::IGameObject> m_scene = nullptr;
//    std::vector<anvil::IGameObject> objects() override;

    const std::string m_id = "play";
    std::vector<std::string> m_textureIds;
};

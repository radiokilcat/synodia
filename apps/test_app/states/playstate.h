#pragma once

#include "gamestate.h"

class GameScene;

class PlayState : public anvil::GameState, public SerializableBase
{
public:
    PlayState();
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

    void to_json(nlohmann::json& j) override;
    void from_json(nlohmann::json& j) override;

private:
    std::unique_ptr<anvil::BaseGameObject> m_scene = nullptr;
//    std::vector<anvil::BaseGameObject> objects() override;

    const std::string m_id = "play";
    std::vector<std::string> m_textureIds;
};

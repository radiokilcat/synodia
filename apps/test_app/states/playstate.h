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

    ~PlayState() 
    {
        if (m_scene)
        {
            delete m_scene;
            m_scene = nullptr;
        }
    }

private:

//    std::vector<GameObject*> m_gameObjects;
    GameScene *m_scene = nullptr;

    const std::string m_id = "play";
    std::vector<std::string> m_textureIds;
};

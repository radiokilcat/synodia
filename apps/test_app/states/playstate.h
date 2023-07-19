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
            m_scene = NULL;
        }
    }

private:

//    std::vector<GameObject*> m_gameObjects;
    GameScene *m_scene = NULL;

    const std::string m_id = "play";
};

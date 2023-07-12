#pragma once

#include "gamestate.h"

class GameScene;

class PlayState : public anvil::GameState
{
public:
    PlayState();
    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getID() override;

private:

//    std::vector<GameObject*> m_gameObjects;
    std::unique_ptr<GameScene> m_scene = nullptr;

    const std::string m_id = "play";
};

#pragma once

#include "anvil.h"

class GameScene : public anvil::BaseGameObject
{
public:
    GameScene() {};
    ~GameScene() {};

    void draw(std::shared_ptr<anvil::Renderer> renderer);
    void update();
    void clean();

    void addGameObject(std::unique_ptr<anvil::GameObject> gameObject);
private:
    std::vector<std::unique_ptr<anvil::GameObject>> m_childs;
};

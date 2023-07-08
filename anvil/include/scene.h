#pragma once


#include "gameobject.h"
#include <memory>

namespace anvil {

class GameScene: public BaseGameObject
{
public:
    GameScene() {};
    ~GameScene() {};

    virtual void draw(std::shared_ptr<Renderer> renderer);
    virtual void update();
    virtual void clean();

    void addGameObject(std::unique_ptr<GameObject> gameObject);

protected:
    std::unique_ptr<GameObject> parent = nullptr;
    std::vector<std::unique_ptr<GameObject>> m_childs;
};

}

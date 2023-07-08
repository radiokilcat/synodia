#include "scene.h"

namespace anvil {


void GameScene::draw(std::shared_ptr<Renderer> renderer)
{
    for (auto& child: m_childs)
    {
        child->draw(renderer);
    }
}

void GameScene::update()
{
    for (auto& child: m_childs)
    {
        child->update();
    }
}

void GameScene::clean()
{
    for (auto& child: m_childs)
    {
        child->clean();
    }
}

void GameScene::addGameObject(std::unique_ptr<GameObject> gameObject)
{
    m_childs.push_back(std::move(gameObject));
}

}


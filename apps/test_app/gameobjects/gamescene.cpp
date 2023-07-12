#include "gamescene.h"

void GameScene::draw(std::shared_ptr<anvil::Renderer> renderer)
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

void GameScene::addGameObject(std::unique_ptr<anvil::GameObject> gameObject)
{
    m_childs.push_back(std::move(gameObject));
}

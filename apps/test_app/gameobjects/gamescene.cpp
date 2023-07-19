#include "gamescene.h"
#include <memory>
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

void GameScene::addGameObject(std::unique_ptr<anvil::BaseGameObject> gameObject)
{
    m_childs.push_back(std::move(gameObject));
}

void GameScene::to_json(nlohmann::json& j)
{
    for (auto &gameObject : m_childs)
    {
        nlohmann::json childJ;
        gameObject->to_json(childJ);
        childJ["class"] = typeid(*gameObject).name();
        j["m_childs"].push_back(childJ);
    }
}

void GameScene::from_json(nlohmann::json& j)
{
    for (auto childJ : j["m_childs"]) {
        std::string className = childJ["class"];
        if (className == "class anvil::TileMap") {
            auto child = new anvil::TileMap;
            child->from_json(childJ);
            std::unique_ptr<anvil::BaseGameObject> parentPtr(child);
            m_childs.push_back(std::move(parentPtr));
        }
        else {
            auto child = new Player();
            child->from_json(childJ);
            std::unique_ptr<anvil::BaseGameObject> parentPtr(child);
            m_childs.push_back(std::move(parentPtr));
        }

    }
}
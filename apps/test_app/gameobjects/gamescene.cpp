#include "gamescene.h"
#include <memory>
#include <iostream>

void GameScene::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    m_tileMap->draw(renderer);
    m_player->draw(renderer);

    for (auto& child: m_childs)
    {
        child->draw(renderer);
    }
}

void GameScene::update()
{
    m_tileMap->update();
    m_player->update();
    int p_x = m_player->getX() + m_player->getWidth() / 2;
    int p_y = m_player->getY() + m_player->getHeight() / 2;

    if (m_player) {
//        std::pair<int, int> tileGridPos = m_tileMap->getTileByPosition(p_x, p_y);
        m_tileMap->setTileOutline(p_x, p_y);
    }
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

void GameScene::load(const anvil::LoaderParams* params)
{

}

void GameScene::to_json(nlohmann::json& j)
{
//    for (auto &gameObject : m_childs)
//    {
//        nlohmann::json childJ;
//        gameObject->to_json(childJ);
//        childJ["class"] = typeid(*gameObject).name();
//        j["m_childs"].push_back(childJ);
//    }
    GameObject::to_json(j);
}

void GameScene::from_json(nlohmann::json& j)
{
//    for (auto childJ : j["m_childs"]) {
//        std::string className = childJ["class"];
//        if (className == "class anvil::TileMap") {
//            auto child = new anvil::TileMap;
//            child->from_json(childJ);
//            std::unique_ptr<anvil::BaseGameObject> parentPtr(child);
//            m_childs.push_back(std::move(parentPtr));
//        }
//        else {
//            auto child = new Player();
//            child->from_json(childJ);
//            std::unique_ptr<anvil::BaseGameObject> parentPtr(child);
//            m_childs.push_back(std::move(parentPtr));
//        }

//    }
    GameObject::from_json(j);
}

bool GameScene::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("GameScene", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<GameScene>();
    });
    return true;
}

void GameScene::addChildObject(std::unique_ptr<BaseGameObject> gameObject)
{
    if (auto player = dynamic_cast<Player*>(gameObject.get())) {
        setPlayer(std::unique_ptr<Player>(player));
        gameObject.release();
    }
    else if (auto tilemap = dynamic_cast<anvil::TileMap*>(gameObject.get())) {
        setTileMap(std::unique_ptr<anvil::TileMap>(tilemap));
        gameObject.release();
    }
    else {
        GameObject::addChildObject(std::move(gameObject));
    }
}

void GameScene::setTileMap(std::unique_ptr<anvil::TileMap> tileMap)
{
    m_tileMap = std::move(tileMap);
}

void GameScene::setPlayer(std::unique_ptr<Player> player)
{
    m_player = std::move(player);
}

#include "gamescene.h"
#include <memory>
#include <iostream>

void GameScene::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    m_tileMap->draw(renderer);

    int p_x = m_player->getX() + m_player->getWidth() / 2;
    int p_y = m_player->getY() + m_player->getHeight() / 2;

    if (m_player) {
        auto tile = m_tileMap->getTileByPosition(p_x, p_y, renderer);
        m_tileMap->outline_tile(tile.first, tile.second, renderer);
    }

    m_player->draw(renderer);
    for (auto& child: m_childs)
    {
        child->draw(renderer);
    }
    anvil::TextureManager::instance()->drawPoint(renderer->getRenderer(), p_x, p_y);
}

void GameScene::update()
{
    m_tileMap->update();
    m_player->update();
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

bool GameScene::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("GameScene", []() -> std::unique_ptr<anvil::BaseGameObject> {
        return std::make_unique<GameScene>();
    });
    return true;
}

void GameScene::setTileMap(std::unique_ptr<anvil::TileMap> tileMap)
{
    m_tileMap = std::move(tileMap);
}

void GameScene::setPlayer(std::unique_ptr<Player> player)
{
    m_player = std::move(player);
}

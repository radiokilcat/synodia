#include "gamescene.h"
#include <memory>
#include <iostream>
#include "stationary.h"
#include "nonplayable.h"
#include "../states/losestate.h"
#include "../states/winstate.h"
#include "logger.h"

void GameScene::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    for (auto& child: m_childs) {
        child->draw(renderer);
    }
}

void GameScene::update()
{
    float p_x = m_player->getX();
    float p_y = m_player->getY();
    float p_w = m_player->getWidth();
    float p_h = m_player->getHeight();

//    m_tileMap->setTileOutline(p_x, p_y);
    m_player->setInverseMove(false);

    if (_dialogState)
    {
        auto delta = anvil::Application::Instance()->getTicks() - _dialogStart;
        if (delta >= 3 * 1000) {
            anvil::Application::Instance()->getStateMachine()->changeState(new WinState);
        }
    }

    for (auto& child: m_childs) {
        child->update();
        if (auto childGameObject = dynamic_cast<Stationary*>(child.get())) {
            if (childGameObject->isIntersect(p_x, p_y, p_w, p_h) && !_dialogState) {
                for (auto& child_: m_childs) {
                    if (auto npc = dynamic_cast<NonPlayable*>(child_.get())) {
                        npc->setVelocity(0, 0);
                    }
                }
                m_player->blockInteractions();
                _dialogState = true;
                _dialogStart = anvil::Application::Instance()->getTicks();

            }
        }
        else if (auto npc = dynamic_cast<NonPlayable*>(child.get())) {
            if (npc->isIntersect(p_x, p_y, p_w, p_h)) {
                 anvil::Application::Instance()->getStateMachine()->changeState(new LoseState);
            }

            auto playerDirection = anvil::Vector2D{float(p_x), float(p_y)};
            npc->moveTowards(playerDirection, 0.2f);
        }
    }
}

void GameScene::clean()
{
    for (auto& child: m_childs) {
        child->clean();
    }
}

void GameScene::load(const anvil::GameObjectData*)
{
}

void GameScene::to_json(nlohmann::json& j) {
    GameObject::to_json(j);
}

void GameScene::from_json(const nlohmann::json& j) {
    GameObject::from_json(j);
}

bool GameScene::registerWithFactory() {
    anvil::GameObjectFactory::instance().registerType("GameScene", []() -> std::shared_ptr<anvil::IGameObject> {
        return std::make_shared<GameScene>();
    });
    return true;
}

void GameScene::addChild(std::shared_ptr<IGameObject> gameObject) {
    if (auto player = std::dynamic_pointer_cast<Player>(gameObject)) {
        setPlayer(player);
    }
    else if (auto tilemap = std::dynamic_pointer_cast<anvil::TileMap>(gameObject)) {
        setTileMap(tilemap);
    }
    else if (auto speech = std::dynamic_pointer_cast<anvil::ScrollableText>(gameObject)) {
        setSpeech(speech);
    }
    GameObject::addChild(gameObject);
}

void GameScene::setTileMap(std::shared_ptr<anvil::TileMap> tileMap) {
    m_tileMap = tileMap;
}

void GameScene::setPlayer(std::shared_ptr<Player> player) {
    m_player = player;
}

void GameScene::setSpeech(std::shared_ptr<anvil::ScrollableText> scrollable) {
    m_scrollable = scrollable;
}

float GameScene::distance(anvil::Vector2D, anvil::Vector2D) {
    return 0;
}

float GameScene::distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;

    return std::sqrt(dx * dx + dy * dy);

}

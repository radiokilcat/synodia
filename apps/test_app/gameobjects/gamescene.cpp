#include "gamescene.h"
#include <memory>
#include "stationary.h"
#include "nonplayable.h"
#include "../states/losestate.h"
#include "../states/winstate.h"
#include "logger.h"
#include "components/MovementIsoComponent.h"
#include "components/Sprite2DComponent.h"

void GameScene::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    for (auto& child: m_children) {
        child->draw(renderer);
    }
    for (auto& child: m_children) {
        child->draw(renderer);
    }
}

void GameScene::update(Uint64 deltaTime)
{
//    m_tileMap->setTileOutline(p_x, p_y);
    m_player->setInverseMove(false);

    if (_dialogState)
    {
        auto delta = anvil::Application::Instance()->getTicks() - _dialogStart;
        if (delta >= 3 * 1000) {
            anvil::Application::Instance()->getStateMachine()->changeState(new WinState);
        }
    }

    for (auto& child: m_children) {
        child->update(deltaTime);
        if (auto npc = dynamic_cast<NonPlayable*>(child.get())) {
            npc->setChaising(false);
            if (npc->chasing()) {
                npc->getComponent<anvil::MovementIsoComponent>()->setVelocityTowardObject(m_player, 0.2f);
            }
        }
        if (auto obstacle = dynamic_cast<Stationary*>(child.get())) {
            if (obstacle->isCollide(m_player) && !_dialogState) {
                for (auto& child_: m_children) {
                    if (auto npc = dynamic_cast<NonPlayable*>(child_.get())) {
                        npc->setChaising(false);
                        npc->getComponent<anvil::MovementIsoComponent>()->setVelocity(anvil::Vector2D{0, 0});
                    }
                }
                m_player->blockInteractions();
                _dialogState = true;
                _dialogStart = anvil::Application::Instance()->getTicks();
                 anvil::Application::Instance()->getStateMachine()->changeState(new WinState);
            }
        }
        else if (auto npc = dynamic_cast<NonPlayable*>(child.get())) {
            if (npc->isCollide(m_player)) {
                 anvil::Application::Instance()->getStateMachine()->changeState(new LoseState);
            }
            // npc->getComponent<anvil::MovementIsoComponent>()->setVelocityTowardObject(m_player, 0.2f);
        }
    }
}

void GameScene::clean() {
}

void GameScene::load() {
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

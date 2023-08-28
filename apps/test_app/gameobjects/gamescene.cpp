#include "gamescene.h"
#include <memory>
#include <iostream>
#include "stationary.h"
#include "nonplayable.h"
#include "../states/losestate.h"
#include "../states/winstate.h"

void GameScene::draw(std::shared_ptr<anvil::Renderer> renderer)
{
    m_tileMap->draw(renderer);
    m_player->draw(renderer);
    m_scrollable->draw(renderer);
    for (auto& child: m_childs)
    {
        child->draw(renderer);
    }
}

void GameScene::update()
{
    m_tileMap->update();
    m_player->update();
    m_scrollable->update();

    int p_x = m_player->getX();
    int p_y = m_player->getY();
    int p_w = m_player->getWidth();
    int p_h = m_player->getHeight();

//    m_tileMap->setTileOutline(p_x, p_y);
    m_player->setInverseMove(false);

    if (_dialogState)
    {
        auto delta = anvil::Application::Instance()->getTicks() - _dialogStart;
        if (delta >= 3 * 1000)
        {
            anvil::Application::Instance()->getStateMachine()->changeState(new WinState);
        }
    }

    for (auto& child: m_childs)
    {
        child->update();
        if(auto childGameObject = dynamic_cast<Stationary*>(child.get()))
        {
            if (childGameObject->isIntersect(p_x, p_y, p_w, p_h) && !_dialogState)
            {
                for (auto& child: m_childs)
                {
                    if (auto npc = dynamic_cast<NonPlayable*>(child.get()))
                    {
                        npc->setVelocity(0, 0);
                    }
                }
                m_player->blockInteractions();
                _dialogState = true;
                _dialogStart = anvil::Application::Instance()->getTicks();

            }
//            m_player->setInverseMove(true);
        }
        else if (auto npc = dynamic_cast<NonPlayable*>(child.get()))
        {
            if(npc->isIntersect(p_x, p_y, p_w, p_h))
            {
                 anvil::Application::Instance()->getStateMachine()->changeState(new LoseState);
            }

            auto playerDirection = anvil::Vector2D{float(p_x), float(p_y)};
            npc->moveTowards(playerDirection, 0.2f);
        }
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
    GameObject::to_json(j);
}

void GameScene::from_json(nlohmann::json& j)
{
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
    else if (auto speech = dynamic_cast<anvil::ScrollableText*>(gameObject.get())) {
        setSpeech(std::unique_ptr<anvil::ScrollableText>(speech));
        gameObject.release();
    } else {
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

void GameScene::setSpeech(std::unique_ptr<anvil::ScrollableText> scrollable) {
    m_scrollable = std::move(scrollable);
}

float GameScene::distance(anvil::Vector2D p1, anvil::Vector2D p2)
{
    return 0;
}

float GameScene::distance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return std::sqrt(dx * dx + dy * dy);

}

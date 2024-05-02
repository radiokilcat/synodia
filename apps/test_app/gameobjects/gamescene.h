#pragma once

#include "anvil.h"
#include "player.h"

class GameScene : public anvil::GameObject
{
public:
    GameScene() {};
    ~GameScene() {};

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update(Uint64 deltaTime) override;
    void clean() override;
    void init() {};

    // for now we can load gameScene via load / from_json
    // TODO: probably need to leave one way
    void load();

    // void to_json(nlohmann::json& j) override;
    // void from_json(const nlohmann::json& j) override;

    static bool registerWithFactory();

    void addChild(std::shared_ptr<IGameObject> gameObject) override;
    void setTileMap(std::shared_ptr<anvil::TileMap> tileMap);
    void setPlayer(std::shared_ptr<Player> player);
    void setSpeech(std::shared_ptr<anvil::ScrollableText> scrollable);

    bool checkCollision(std::shared_ptr<anvil::IGameObject>& a, std::shared_ptr<anvil::IGameObject>& b);

    float distance(anvil::Vector2D p1, anvil::Vector2D p2);
    float distance(float x1, float y1, float x2, float y2);


private:
    std::shared_ptr<anvil::TileMap> m_tileMap;
    std::shared_ptr<Player> m_player;
    std::shared_ptr<anvil::ScrollableText> m_scrollable;
    bool _dialogState = false;
    Uint64 _dialogStart;
};

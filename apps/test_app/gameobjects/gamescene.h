#pragma once

#include "anvil.h"
#include "player.h"

class GameScene : public anvil::GameObject
{
public:
    GameScene() {};
    ~GameScene() {};

    void draw(std::shared_ptr<anvil::Renderer> renderer) override;
    void update() override;
    void clean() override;

    // for now we can load gameScene via load / from_json
    // TODO: probably need to leave one way
    void load(const anvil::LoaderParams* params) override;

    void to_json(nlohmann::json& j) override;
    void from_json(nlohmann::json& j) override;

    static bool registerWithFactory();

    void addChildObject(std::unique_ptr<BaseGameObject> gameObject) override;
    void setTileMap(std::unique_ptr<anvil::TileMap> tileMap);
    void setPlayer(std::unique_ptr<Player> player);
    void setSpeech(std::unique_ptr<anvil::ScrollableText> scrollable);

    bool checkCollision(std::unique_ptr<anvil::BaseGameObject>& a, std::unique_ptr<anvil::BaseGameObject>& b);

    float distance(anvil::Vector2D p1, anvil::Vector2D p2);
    float distance(float x1, float y1, float x2, float y2);


private:
    std::unique_ptr<anvil::TileMap> m_tileMap;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<anvil::ScrollableText> m_scrollable;
    bool _dialogState = false;
    Uint64 _dialogStart;
};

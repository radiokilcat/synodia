#pragma once

#include "gameobject.h"

namespace anvil {

class Tile : GameObject {
public:
    Tile(LoaderParams* params);
    Tile() {};
    void draw();
    void update();
    void clean();

    int getScreenX();
    int getScreenY();
    int getWidth();
    int getHeight();

private:
    bool outlined = false;
};

class TileMap : public GameObject
{
public:
    TileMap();
    void draw_tile(std::string id, int x, int y, std::shared_ptr<Renderer> renderer);
    void draw(std::shared_ptr<Renderer> renderer) override;
    void clean() override;
    void update() override;

    void from_json(nlohmann::json& j) override;
    void to_json(nlohmann::json& j) override;

    void loadTiles();

    std::pair<int, int> findScreenPosition(std::string textureId, int x, int y);
    std::pair<int, int> getTileByPosition(float x, float y, std::shared_ptr<Renderer> renderer) ;

    ~TileMap() {};
    static bool registerWithFactory();
private:
    std::vector<std::vector<std::string>> m_grid;

    void outline_tile(int x, int y, std::shared_ptr<Renderer> renderer);

    std::map<std::pair<int, int>, Tile> m_tiles;
    int m_tileWidth;
    int m_tileHeight;

    int m_screenWidth;
    int m_screenHeight;

    int m_startX;
    int m_startY;
};

}

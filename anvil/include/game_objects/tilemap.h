#pragma once

#include "gameobject.h"

namespace anvil {

struct Tile {
    std::string textureId;
    bool outlined = false;
    Tile(const std::string& textureId, bool outlined = false)
        : textureId(textureId), outlined(outlined) {}
    Tile() = default;
};

class TileMap : public GameObject
{
public:
    TileMap();
    ~TileMap() {};
    void draw_tile(Tile& tile, int x, int y, std::shared_ptr<Renderer> renderer);
    void draw(std::shared_ptr<Renderer> renderer) override;
    void clean() override;
    void init() override {};
    void update(Uint64 deltaTime) override;

    void from_json(const nlohmann::json& j) override;
    void to_json(nlohmann::json& j) override;

    void loadTiles();
    void generateRandomLayout(int dimension, std::vector<std::string> elements);

    std::pair<int, int> getTileScreenPosition(int x, int y);
    std::pair<float, float> findNearestTileCenter(float x, float y, Direction direction);
    std::pair<int, int> getTileByPosition(float x, float y) ;


    void setTileOutline(int x, int y);
    static bool registerWithFactory();
private:
    std::vector<std::vector<std::string>> m_grid;
    std::map<std::pair<int, int>, Tile> m_tiles;

    int m_tileWidth;
    int m_tileHeight;

    int m_startX;
    int m_startY;
};

}

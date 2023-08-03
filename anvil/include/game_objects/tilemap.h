#pragma once

#include "gameobject.h"

namespace anvil {

class TileMap: public GameObject
{
public:
    TileMap();
    void draw_tile(std::string id, int x, int y, std::shared_ptr<Renderer> renderer);
    void draw(std::shared_ptr<Renderer> renderer) override;
    void clean() override;
    void from_json(nlohmann::json& j) override;
    void to_json(nlohmann::json& j) override;
    ~TileMap() {};
    static bool registerWithFactory();
private:
    std::vector<std::vector<std::string>> m_grid;

    int m_tileWidth;
    int m_tileHeight;

    int m_screenWidth;
    int m_screenHeight;

    int m_startX;
    int m_startY;
};

}

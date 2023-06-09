#pragma once

#include "gameobject.h"

namespace anvil {

class TileMap: public GameObject
{
public:
    TileMap(LoaderParams* params);
    void draw_tile(std::string id, int x, int y, std::shared_ptr<Renderer>& renderer);
    void draw(std::shared_ptr<Renderer>& renderer) override;
    void clean() override;

    ~TileMap() {};
private:
    int m_tileWidth;
    int m_tileHeight;

    int m_screenWidth;
    int m_screenHeight;

    int m_startX;
    int m_startY;
};

}

#include "tilemap.h"
#include "renderer.h"
#include "application.h"

std::vector<std::vector<std::string>> grid = {
    {"grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass"},
    {"grass", "water", "water", "water", "water", "water", "water", "water", "water", "grass"},
    {"grass", "hill", "water", "water", "water", "water", "water", "water", "water", "grass"},
    {"grass", "water", "water", "water", "water", "water", "water", "water", "water", "grass"},
    {"grass", "water", "water", "sand", "water", "water", "water", "water", "water", "grass"},
    {"grass", "water", "water", "sand", "water", "water", "water", "water", "water", "grass"},
    {"grass", "water", "water", "water", "water", "water", "water", "water", "water", "grass"},
    {"grass", "water", "water", "water", "water", "water", "water", "sand", "water", "grass"},
    {"grass", "water", "water", "water", "water", "water", "water", "water", "water", "grass"},
    {"grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass"}
};

const int MAX_TILE_HEIGHT = 90;

namespace anvil {

TileMap::TileMap(LoaderParams* params)
  :GameObject(params)
  , m_tileWidth(100)
  , m_tileHeight(50)
  , m_startX(Application::Instance()->getScreenWidth() / 2 - m_tileWidth / 2)
  , m_startY(10)
{
}

TileMap::TileMap()
    :GameObject()
    , m_tileWidth(100)
    , m_tileHeight(50)
    , m_startX(Application::Instance()->getScreenWidth() / 2 - m_tileWidth / 2)
    , m_startY(10)
{
}


void TileMap::draw_tile(std::string id, int x, int y, std::shared_ptr<Renderer> renderer)
{
    auto tileHeight = TextureManager::instance()->textureSize(id).second;

    auto tileX = m_startX + (x - y) * m_tileWidth / 2;
    auto tileY = m_startY + (x + y) * m_tileHeight / 2;
    auto tileZ = MAX_TILE_HEIGHT - tileHeight;
    TextureManager::instance()->draw(id, (Uint32)tileX, (Uint32)tileY + (Uint32)tileZ,
                                          m_tileWidth, tileHeight,
                                          renderer->getRenderer());
}

void TileMap::draw(std::shared_ptr<Renderer> renderer)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid.size(); j++)
        {
            draw_tile(grid[i][j], i, j, renderer);
        }
    }
}

void TileMap::clean()
{
}

void TileMap::from_json(nlohmann::json& j)
{
    GameObject::from_json(j);
}
void TileMap::to_json(nlohmann::json& j) {
    GameObject::to_json(j);

}

}

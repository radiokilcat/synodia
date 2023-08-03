#include "game_objects/tilemap.h"
#include "renderer.h"
#include "application.h"
#include "game_objects/GameObjectsFactory.h"


const int MAX_TILE_HEIGHT = 90;

namespace anvil {

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
    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid.size(); j++)
        {
            draw_tile(m_grid[i][j], i, j, renderer);
        }
    }
}

void TileMap::clean()
{
}

void TileMap::from_json(nlohmann::json& j)
{
    if (j.count("grid") > 0 && j["grid"].is_array()) {
        for (auto& row : j["grid"]) {
            if (row.is_array()) {
                std::vector<std::string> rowVec;

                for (auto& elem : row) {
                    rowVec.push_back(elem.get<std::string>());
                }

                m_grid.push_back(rowVec);
            }
        }
    GameObject::from_json(j);
    }
}

void TileMap::to_json(nlohmann::json& j)
{
    j["grid"] = m_grid;
    GameObject::to_json(j);
}

bool TileMap::registerWithFactory() {
    GameObjectFactory::instance().registerType("TileMap", []() -> std::unique_ptr<BaseGameObject> {
        return std::make_unique<TileMap>();
    });
    return true;
}

}

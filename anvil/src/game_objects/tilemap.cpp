#include "game_objects/tilemap.h"
#include "renderer.h"
#include "application.h"
#include "game_objects/GameObjectsFactory.h"


const int MAX_TILE_HEIGHT = 90;

namespace anvil {

TileMap::TileMap()
  :GameObject()
  , m_tileWidth(128)
  , m_tileHeight(64)
  , m_startX(Application::Instance()->getScreenWidth() / 2 - m_tileWidth / 2)
  , m_startY(-200)
{
}


void TileMap::draw_tile(Tile& tile, int x, int y, std::shared_ptr<Renderer> renderer)
{
    auto tileHeight = TextureManager::instance()->textureSize(tile.textureId).second;

    auto tileX = m_startX + (x - y) * m_tileWidth / 2;
    auto tileY = m_startY + (x + y) * m_tileHeight / 2;
    auto tileZ = MAX_TILE_HEIGHT - tileHeight;
    TextureManager::instance()->draw(tile.textureId, (Uint32)tileX, (Uint32)tileY + (Uint32)tileZ,
                                          m_tileWidth, tileHeight,
                                          renderer->getRenderer());
    if (tile.outlined)
    {
        SDL_FPoint p1 = { tileX + m_tileWidth / 2, tileY + tileZ };
        SDL_FPoint p2 = { tileX + m_tileWidth / 2 + m_tileWidth / 2, tileY + tileZ  + m_tileHeight / 2 };
        SDL_FPoint p3 = { tileX + m_tileWidth / 2, tileY + tileZ  + m_tileHeight };
        SDL_FPoint p4 = { tileX - m_tileWidth / 2 + m_tileWidth / 2, tileY + tileZ + m_tileHeight / 2 };

        SDL_FPoint points[5] = { p1, p2, p3, p4, p1 };

        SDL_SetRenderDrawColor(renderer->getRenderer(), 255, 0, 0, 255);

        SDL_RenderLines(renderer->getRenderer(), points, 5);

        SDL_SetRenderDrawColor(renderer->getRenderer(), 100, 149, 237, 255);
    }
    //TODO: bring out all post-draw things to it's own function in BaseGameObject(?)
    tile.outlined = false;
}

void TileMap::draw(std::shared_ptr<Renderer> renderer)
{
    for (auto& it: m_tiles)
    {
        draw_tile(it.second, it.first.first, it.first.second, renderer);
    }
}


void TileMap::clean()
{
}

void TileMap::update()
{

}
void TileMap::setTileOutline(int x, int y)
{
    auto gridPosition = getTileByPosition(x, y);
    auto it = m_tiles.find(gridPosition);

    if (it != m_tiles.end()) {
        it->second.outlined = true;
    }
}

std::pair<int, int> TileMap::getTileByPosition(float x, float y)
{
    x -= m_startX;
    y -= m_startY;

    int gridX = (x / (m_tileWidth / 2) + y / (m_tileHeight / 2)) / 2;
    int gridY = (y / (m_tileHeight / 2) - x / (m_tileWidth / 2)) / 2;

    return {gridX, gridY};
}

//void TileMap::loadTiles()
//{
//    for (int i = 0; i < grid.size(); i++)
//    {
//        for (int j = 0; j < grid.size(); j++)
//        {
//            std::stringstream ss;
//            ss << "tile_" << i << "_" << j;
//            auto position = findScreenPosition(grid[i][j], i, j);
//            m_tiles[std::make_pair(i, j)] = Tile(new LoaderParams(position.first, position.second, m_tileWidth, m_tileHeight, ss.str()));
//        }
//    }
//}

std::pair<int, int> TileMap::findScreenPosition(std::string textureId, int x, int y)
{
    auto tileHeight = TextureManager::instance()->textureSize(textureId).second;

    auto tileX = m_startX + (x - y) * m_tileWidth / 2;
    auto tileY = m_startY + (x + y) * m_tileHeight / 2;
    auto zOffset = MAX_TILE_HEIGHT - tileHeight;

    return std::make_pair(tileX, tileY + zOffset);
}


void TileMap::from_json(nlohmann::json& j)
{
    if (j.count("grid") > 0 && j["grid"].is_array()) {
        int rowIndex = 0;
        for (auto& row : j["grid"]) {
            if (row.is_array()) {
                std::vector<std::string> rowVec;
                int colIndex = 0;

                for (auto& elem : row) {
                    rowVec.push_back(elem.get<std::string>());

                    Tile tile(elem.get<std::string>(), false);
                    m_tiles[std::make_pair(rowIndex, colIndex)] = tile;

                    colIndex++;
                }
                m_grid.push_back(rowVec);
            }
            rowIndex++;
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

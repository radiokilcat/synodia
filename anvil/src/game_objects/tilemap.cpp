#include "tilemap.h"
#include "renderer.h"
#include "application.h"
#include  <random>

#include "texturemanager.h"
#include "GameObjectsFactory.h"


const int MAX_TILE_HEIGHT = 90;

namespace anvil {

TileMap::TileMap()
  :GameObject()
  , m_tileWidth(256)
  , m_tileHeight(128)
  , m_startX(Application::Instance()->getScreenWidth() / 2 - m_tileWidth / 2)
  , m_startY(-700)
{
}


void TileMap::draw_tile(Tile& tile, int x, int y, std::shared_ptr<Renderer> renderer)
{
    auto tileHeight = TextureManager::instance()->textureSize(tile.textureId).second;

    auto tileX = m_startX + (x - y) * m_tileWidth / 2;
    auto tileY = m_startY + (x + y) * m_tileHeight / 2;
//    auto tileZ = MAX_TILE_HEIGHT - tileHeight;
//    std::cout << "tile grid: " << x << " : " << y << "   tile coords in draw_tile: " <<  tileX << " : " << tileY << std::endl;
    TextureManager::instance()->draw(tile.textureId, static_cast<float>(tileX), static_cast<float>(tileY),
                                          static_cast<float>(m_tileWidth), static_cast<float>(tileHeight),
                                          renderer->getRenderer());
    if (tile.outlined)
    {
        SDL_FPoint p1 = { tileX + m_tileWidth / 2, tileY };
        SDL_FPoint p2 = { tileX + m_tileWidth / 2 + m_tileWidth / 2, tileY  + m_tileHeight / 2 };
        SDL_FPoint p3 = { tileX + m_tileWidth / 2, tileY  + m_tileHeight };
        SDL_FPoint p4 = { tileX - m_tileWidth / 2 + m_tileWidth / 2, tileY + m_tileHeight / 2 };

        SDL_FPoint points[5] = { p1, p2, p3, p4, p1 };

        SDL_SetRenderDrawColor(renderer->getRenderer(), 255, 0, 0, 255);

        SDL_RenderLines(renderer->getRenderer(), points, 5);

        SDL_SetRenderDrawColor(renderer->getRenderer(), 100, 149, 237, 255);
    }
    //TODO: bring out all post-draw things to it's own function in IGameObject(?)
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

void TileMap::update(Uint64 deltaTime)
{

}

void TileMap::setTileOutline(int x, int y)
{
    auto gridPosition = getTileByPosition(x, y);
//    std::cout << "tileGridPos in outline: " <<  gridPosition.first << " : " << gridPosition.second << std::endl;
    auto it = m_tiles.find(gridPosition);

    if (it != m_tiles.end()) {
        it->second.outlined = true;
    }
}

std::pair<int, int> TileMap::getTileByPosition(float sX, float sY)
{
    int gX = ((sX - m_startX) / (m_tileWidth / 2) + (sY - m_startY) / (m_tileHeight / 2)) / 2;
    int gY = ((sY - m_startY) / (m_tileHeight / 2) - (sX - m_startX) / (m_tileWidth / 2)) / 2;
//    sX -= m_startX;
//    sY -= m_startY;

//    int gX = 2 * sX / (m_tileWidth + m_tileHeight);
//    int gY = 2 * sY / (m_tileHeight - m_tileWidth);

    return {gX, gY};
}


std::pair<int, int> TileMap::getTileScreenPosition(int x, int y)
{
//    auto tileHeight = 128;
//    auto gridPosition = getTileByPosition(x, y);

    auto screenX = m_startX + (x - y) * m_tileWidth / 2;
    auto screenY = m_startY + (x + y) * m_tileHeight / 2;
    return std::make_pair(screenX, screenY);
}

std::pair<float, float> TileMap::findNearestTileCenter(float x, float y, Direction direction)
{
    auto gridPos = getTileByPosition(x, y);

    auto gridX = gridPos.first;
    auto gridY = gridPos.second;

    switch(direction)
    {
    case Direction::Up:
        gridY--;
        break;
    case Direction::Down:
        gridY++;
        break;
    case Direction::Left:
        gridX--;
        break;
    case Direction::Right:
        gridX++;
        break;
    case Direction::Static:
        return std::make_pair(x, y);
        break;
    }

    std::pair<float, float> screenPos = getTileScreenPosition(x, y);

    return std::make_pair(screenPos.first, screenPos.second + m_tileHeight / 2);

//    float centerX = m_startX + (gridX - gridY) * m_tileWidth / 2 + m_tileWidth / 2;
//    float centerY = m_startY + (gridX + gridY) * m_tileHeight / 2 + m_tileHeight / 2;

//    return std::make_pair(centerX, centerY);
}


void TileMap::generateRandomLayout(int dimension, std::vector<std::string> elements) {
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
    
    gen = std::mt19937(rd());
    dis = std::uniform_int_distribution<>(0, elements.size() - 1);

    m_tiles.clear();
    m_grid.clear();
    m_grid.resize(dimension, std::vector<std::string>(dimension));
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            std::string tileType = elements[dis(gen)];
            m_grid[i][j] = tileType;

            Tile tile(tileType, false);
            m_tiles[std::make_pair(i, j)] = tile;
        }
    }
}
    
void TileMap::from_json(const nlohmann::json& j)
{
    const auto& fillType = j.at("fillType");
    if (fillType =="random") {
        generateRandomLayout(j.at("dimension"), j.at("elements"));
    }
    GameObject::from_json(j);
}

void TileMap::to_json(nlohmann::json& j)
{
    j["grid"] = m_grid;
    GameObject::to_json(j);
}

bool TileMap::registerWithFactory() {
    GameObjectFactory::instance().registerType("TileMap", []() -> std::unique_ptr<IGameObject> {

        return std::make_unique<TileMap>();
    });
    return true;
}

}

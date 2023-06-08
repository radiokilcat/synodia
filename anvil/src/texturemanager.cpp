#include <SDL_image.h>

#include "texturemanager.h"
#include <iostream>

static anvil::TextureManager* instance_;

namespace anvil {

TextureManager::TextureManager()
{
}

TextureManager* TextureManager::instance()
{
    if(instance_ == 0)
    {
        instance_ = new TextureManager;
        return instance_;
    }
    return instance_;
}

bool TextureManager::loadTexture(std::string filename, std::string id, SDL_Renderer *renderer)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename.c_str());
    if (texture != 0)
    {
        m_texture_map[id] = texture;
        return true;
    }
    std::cout << SDL_GetError()  << " _+_  " << filename.c_str() << std::endl;

    return false;
}

void TextureManager::draw(std::string id, int x, int y,
                          int width, int height,
                          SDL_Renderer *renderer)
{
    SDL_FRect srcRect;
    SDL_FRect destRect;

    srcRect.x = 0.f;
    srcRect.y = 0.f;
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;

    destRect.x = x;
    destRect.y = y;

    SDL_RenderTexture(renderer, m_texture_map[id], &srcRect, &destRect);
}

void TextureManager::drawFrame(std::string id, int x, int y,
                               int width, int height,
                               int currentRow, int currentFrame,
                               SDL_Renderer *renderer)
{
    SDL_FRect srcRect;
    SDL_FRect destRect;

    srcRect.x = width * currentFrame;
    srcRect.y = height * (currentRow - 1);
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;

    destRect.x = x;
    destRect.y = y;

    SDL_RenderTexture(renderer, m_texture_map[id], &srcRect, &destRect);
}

void TextureManager::clearFromTextureMap(std::string id)
{
    m_texture_map.erase(id);
}

}

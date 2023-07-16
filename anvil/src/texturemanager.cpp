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

bool TextureManager::loadTexture(std::string filename, std::string id, SDL_Renderer* renderer)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename.c_str());
    if (texture != 0)
    {
        m_texture_map[id] = texture;
        return true;
    }
    std::cout << SDL_GetError() << " : " << filename.c_str() << std::endl;

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
    srcRect.w = destRect.w = (float)width;
    srcRect.h = destRect.h = (float)height;

    destRect.x = (float)x;
    destRect.y = (float)y;

    SDL_RenderTexture(renderer, m_texture_map[id], &srcRect, &destRect);
}

void TextureManager::drawFrame(std::string id, int x, int y,
                               int width, int height,
                               int currentRow, int currentFrame,
                               SDL_Renderer *renderer)
{
    SDL_FRect srcRect;
    SDL_FRect destRect;

    srcRect.x = (float)width * currentFrame;
    srcRect.y = (float)height * (currentRow - 1);
    srcRect.w = destRect.w = (float)width;
    srcRect.h = destRect.h = (float)height;

    destRect.x = (float)x;
    destRect.y = (float)y;

    SDL_RenderTexture(renderer, m_texture_map[id], &srcRect, &destRect);
}

void TextureManager::drawText(std::string id, std::string text,
                              TTF_Font* font, SDL_Color color,
                              int x, int y, int width, int height,
                              SDL_Renderer* renderer)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_FRect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = width;
    Message_rect.h = height;

    SDL_RenderTexture(renderer, Message, NULL, &Message_rect);
}

void TextureManager::clearFromTextureMap(std::string id)
{
    m_texture_map.erase(id);
}

std::pair<int, int> TextureManager::textureSize(std::string id)
{
    int w, h;
    auto size = SDL_QueryTexture(m_texture_map[id], NULL, NULL, &w, &h);
    return std::pair<int, int>(w, h);
}

}

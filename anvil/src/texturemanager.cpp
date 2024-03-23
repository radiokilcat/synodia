#include <SDL_image.h>

#include "texturemanager.h"
#include <iostream>
#include <vector2d.h>
#include "logger.h"
#include <fmt/format.h>

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

void TextureManager::draw(std::string id, float x, float y,
                          float width, float height,
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

void TextureManager::drawFrame(std::string id, float x, float y,
                               float width, float height,
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


void TextureManager::drawFrameScaled(std::string id, float scale, float x, float y,
    float width, float height,
    int currentRow, int currentFrame,
    SDL_Renderer* renderer)
{
    SDL_FRect srcRect;
    SDL_FRect destRect;

    srcRect.x = (float)width * currentFrame;
    srcRect.y = (float)height * (currentRow - 1);
    srcRect.w = (float)width;
    srcRect.h = (float)height;

    destRect.x = (float)x;
    destRect.y = (float)y;
    destRect.w = (float)width * scale;
    destRect.h = (float)height * scale;

    SDL_RenderTexture(renderer, m_texture_map[id], &srcRect, &destRect);
}


void TextureManager::drawText(std::string id, std::string text,
                              TTF_Font* font, SDL_Color color,
                              float x, float y, float width, float height,
                              SDL_Renderer* renderer)
{
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_FRect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = width;
    Message_rect.h = height;

    SDL_RenderTexture(renderer, Message, NULL, &Message_rect);
    SDL_DestroySurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}


void TextureManager::drawTextWrapped(std::string id, Uint32 wrapLength, std::string text,
    TTF_Font* font, SDL_Color color,
    float x, float y, float width, float height,
    SDL_Renderer* renderer)
{
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrapLength);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_FRect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = width;
    Message_rect.h = height;

    SDL_RenderTexture(renderer, Message, NULL, &Message_rect);
    SDL_DestroySurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void TextureManager::drawTextMultiline(
    SDL_Renderer* renderer,
    TTF_Font* font,
    std::vector<std::string> &m_lines,
    SDL_Color color,
    int startLine,
    int lineCount,
    int lineHeight,
    int padding,
    Vector2D windowPosition,
    int windowWidth,
    int windowHeight,
    SDL_Color backgroundColor)
{
    SDL_Surface* target = SDL_CreateSurface(
        windowWidth,
        windowHeight,
        SDL_PIXELFORMAT_RGB444);
    SDL_FillSurfaceRect(target, NULL, SDL_MapRGB(target->format, backgroundColor.r, backgroundColor.g, backgroundColor.b));
    for (int i = startLine; i < startLine + lineCount; i++) {
        SDL_Surface* surfaceMessage =
            TTF_RenderUTF8_Blended(font, m_lines[i].c_str(), color);
        
        SDL_Rect Message_rect;
        Message_rect.x = padding;
        Message_rect.y = padding + (i % lineCount) * lineHeight;
        Message_rect.w = std::min(windowWidth - padding, (int)(m_lines[i].length() * 14));
        Message_rect.h = lineHeight;

        SDL_BlitSurfaceScaled(surfaceMessage, NULL, target, &Message_rect);

        SDL_DestroySurface(surfaceMessage);
    }
    SDL_FRect dst;
    dst.x = windowPosition.x();
    dst.y = windowPosition.y();
    dst.h = static_cast<float>(lineHeight) * static_cast<float>(lineCount);
    dst.w = static_cast<float>(windowWidth);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, target);
    SDL_RenderTexture(renderer, Message, NULL, &dst);
    SDL_DestroyTexture(Message);
    SDL_DestroySurface(target);
}


void TextureManager::drawPoint(SDL_Renderer* renderer,
                                       float x1, float y1)
{
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderPoint(renderer, x1, y1);
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, SDL_ALPHA_OPAQUE);
}

void TextureManager::drawLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2)
{
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, SDL_ALPHA_OPAQUE);
}

void TextureManager::drawQuadrilateral(SDL_Renderer* renderer,
                       float x1, float y1,
                       float x2, float y2,
                       float x3, float y3,
                       float x4, float y4)
{
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_RenderLine(renderer, x2, y2, x3, y3);
    SDL_RenderLine(renderer, x3, y3, x4, y4);
    SDL_RenderLine(renderer, x4, y4, x1, y1);
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, SDL_ALPHA_OPAQUE);
}

void TextureManager::drawRect(SDL_Renderer* renderer,
    float x, float y,
    float w, float h,
    SDL_Color color = {0, 0, 0})
{
    SDL_Color oldColor { 0, 0, 0 };
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    const SDL_FRect rect { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, SDL_ALPHA_OPAQUE);
}

void TextureManager::clearFromTextureMap(std::string id)
{
    m_texture_map.erase(id);
}

std::pair<int, int> TextureManager::textureSize(std::string id)
{
    int w, h;
    SDL_QueryTexture(m_texture_map[id], NULL, NULL, &w, &h);
    return std::pair<int, int>(w, h);
}

}

#pragma once

#include <memory>
#include <string>
#include "ITexture.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

namespace anvil {

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual bool init(void* window, int width, int height) = 0;
    virtual void clear() = 0;
    virtual void present() = 0;
    virtual void drawTexture() = 0;
    virtual void setLogicalSize(int width, int height) = 0;
    virtual void renderTextureRotated(
        ITexture* texture,
        const SDL_FRect* srcRect,
        const SDL_FRect* dstRect,
        double angle,
        const SDL_FPoint* center,
        SDL_RendererFlip flip) = 0;
    virtual std::shared_ptr<ITexture> loadTextureFromFile(const std::string& path) = 0;
    virtual std::shared_ptr<ITexture> createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color) = 0;
    virtual void fillRect(const SDL_FRect& rect, SDL_Color color) = 0;

};

}
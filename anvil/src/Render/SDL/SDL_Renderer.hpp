#pragma once

#include "../IRenderer.hpp"

#include <SDL3/SDL.h>

namespace anvil {

class SDLRenderer : public IRenderer {
public:
    bool init(void* window, int width, int height) override;
    void clear() override;
    void present() override;
    void drawTexture(/* your params */) override;
    SDL_Renderer* getRawRenderer() const;
    void renderTextureRotated(
        ITexture* texture,
        const SDL_FRect* srcRect,
        const SDL_FRect* dstRect,
        double angle,
        const SDL_FPoint* center,
        SDL_RendererFlip flip) override;
    std::shared_ptr<ITexture> loadTextureFromFile(const std::string &filePath);
    std::shared_ptr<ITexture> createTextTexture(const std::string &text, TTF_Font *font, SDL_Color color);
    void setLogicalSize(int width, int height) override;

private:
    SDL_Renderer* m_renderer = nullptr;
};

}
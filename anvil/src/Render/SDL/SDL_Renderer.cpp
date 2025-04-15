#include <SDL3/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "SDL_Renderer.hpp"
#include "SDL_Texture.hpp"

#include "../../Logger/Logger.h"

namespace anvil {

bool SDLRenderer::init(void* windowPtr, int width, int height) {
    SDL_Window* window = static_cast<SDL_Window*>(windowPtr);

    m_renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        Logger::Err("Failed to create SDL Renderer: {}", SDL_GetError());
        return false;
    }

    if (SDL_SetRenderVSync(m_renderer, SDL_TRUE) != 0) {
        Logger::Err("Warning: VSync could not be enabled! SDL_Error: ", SDL_GetError());
    }

    if (SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND) != 0) {
        Logger::Err("Warning: Failed to set blend mode! SDL_Error: ", SDL_GetError());
    }
    // Set default background color
    // SDL_Color backgroundColor = {30, 30, 46, 255};
    // SDL_Color textColor = {255, 255, 255, 255};
    // SDL_Color barBackgroundColor = {42, 42, 58, 255};
    // SDL_Color barFillColor = {127, 209, 185, 255};
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 46, SDL_ALPHA_OPAQUE);
    return true;
}

void SDLRenderer::clear() {
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::present() {
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::setLogicalSize(int width, int height) {
    if (SDL_SetRenderLogicalPresentation(m_renderer, width, height,
        SDL_LOGICAL_PRESENTATION_STRETCH, SDL_SCALEMODE_LINEAR) != 0) {
        Logger::Err("Failed to set logical presentation: ", SDL_GetError());
    }
}

void SDLRenderer::fillRect(const SDL_FRect &rect, SDL_Color color) {
    Uint8 originalR, originalG, originalB, originalA;
    SDL_GetRenderDrawColor(m_renderer, &originalR, &originalG, &originalB, &originalA);

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_renderer, &rect);

    SDL_SetRenderDrawColor(m_renderer, originalR, originalG, originalB, originalA);
}

void SDLRenderer::drawTexture(/* your parameters */) {
    // Placeholder for future implementation
    // You might accept SDL_Texture*, source & destination rects, rotation, flip, etc.
}

SDL_Renderer* SDLRenderer::getRawRenderer() const {
    return m_renderer;
}

void SDLRenderer::renderTextureRotated(ITexture *texture, const SDL_FRect *srcRect, const SDL_FRect *dstRect, double angle, const SDL_FPoint *center, SDL_RendererFlip flip)
{
    SDLTexture* sdlTex = dynamic_cast<SDLTexture*>(texture);
    if (!sdlTex || !m_renderer) {
        SDL_Log("SDLRenderer::renderTextureRotated: Invalid texture or renderer");
        return;
    }

    SDL_RenderTextureRotated(
        m_renderer,
        sdlTex->getSDLTexture(),
        srcRect,
        dstRect,
        angle,
        center,
        flip
    );
}

std::shared_ptr<ITexture> SDLRenderer::loadTextureFromFile(const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load surface: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Texture* sdlTex = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_DestroySurface(surface);

    if (!sdlTex) {
        SDL_Log("Failed to create SDL texture: %s", SDL_GetError());
        return nullptr;
    }

    return std::make_shared<SDLTexture>(sdlTex);
}

std::shared_ptr<ITexture> SDLRenderer::createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        Logger::Log("TTF_RenderUTF8_Blended failed: {}", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* sdlTex = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_DestroySurface(surface);

    if (!sdlTex) {
        Logger::Log("Failed to create SDL_Texture from surface");
        return nullptr;
    }

    return std::make_shared<SDLTexture>(sdlTex);
}



}
#include "SDL_Texture.hpp"
#include <SDL3/SDL.h>

namespace anvil {

SDLTexture::SDLTexture(SDL_Texture* texture)
    : m_texture(texture) {
    if (m_texture) {
        float w = 0, h = 0;
        SDL_GetTextureSize(m_texture, &w, &h);
        m_width = static_cast<int>(w);
        m_height = static_cast<int>(h);
    }
}

SDLTexture::~SDLTexture() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

SDL_Texture* SDLTexture::getSDLTexture() const {
    return m_texture;
}

int SDLTexture::getWidth() const {
    return m_width;
}

int SDLTexture::getHeight() const {
    return m_height;
}

}
#pragma once

#include "../ITexture.hpp"
#include <SDL3/SDL.h>

namespace anvil {

class SDLTexture : public ITexture {
public:
    SDLTexture(SDL_Texture* texture);
    ~SDLTexture();

    SDL_Texture* getSDLTexture() const;

    int getWidth() const override;
    int getHeight() const override;

private:
    SDL_Texture* m_texture = nullptr;
    int m_width = 0;
    int m_height = 0;
};

}
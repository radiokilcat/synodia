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
    std::shared_ptr<ITexture> loadTextureFromFile(const std::string &filePath);
    void setLogicalSize(int width, int height) override;

private:
    SDL_Renderer* m_renderer = nullptr;
};

}
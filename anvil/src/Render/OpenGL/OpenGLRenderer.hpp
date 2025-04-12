#pragma once

#include <memory>
#include <string>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "../IRenderer.hpp"

namespace anvil {

class OpenGLRenderer : public IRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();
    bool init(void* window, int width, int height) override;
    void clear() override;
    void present() override;
    void drawTexture(/* your params */) override;
    void setLogicalSize(int width, int height) override;
    void renderTextureRotated(
        ITexture* texture,
        const SDL_FRect* srcRect,
        const SDL_FRect* dstRect,
        double angle,
        const SDL_FPoint* center,
        SDL_RendererFlip flip
    ) override;
    std::shared_ptr<ITexture> loadTextureFromFile(const std::string& path);
    std::shared_ptr<ITexture> createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color);
    void fillRect(const SDL_FRect& rect, SDL_Color color) override;


private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;

    void setup2DProjection(int width, int height);
    void initGL();
};
}
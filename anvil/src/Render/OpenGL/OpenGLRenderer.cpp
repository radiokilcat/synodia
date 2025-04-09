#include "OpenGLRenderer.hpp"
#include "OpenGLTexture.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>

namespace anvil {

OpenGLRenderer::OpenGLRenderer() {}

OpenGLRenderer::~OpenGLRenderer() {
    if (m_glContext) {
        SDL_GL_DeleteContext(m_glContext);
    }
}

bool OpenGLRenderer::init(void* windowPtr, int width, int height) {
    m_window = static_cast<SDL_Window*>(windowPtr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        return false;
    }

    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1); // VSync

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setup2DProjection(width, height);
    return true;
}

void OpenGLRenderer::setup2DProjection(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1); // top-left origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLRenderer::clear() {
    glClearColor(100 / 255.f, 149 / 255.f, 237 / 255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::present() {
    SDL_GL_SwapWindow(m_window);
}

void OpenGLRenderer::drawTexture()
{
}

void OpenGLRenderer::setLogicalSize(int width, int height)
{
}

void OpenGLRenderer::renderTextureRotated(
    ITexture* texture,
    const SDL_FRect* srcRect,
    const SDL_FRect* dstRect,
    double angle,
    const SDL_FPoint* center,
    SDL_RendererFlip flip)
{
    auto* glTexture = dynamic_cast<OpenGLTexture*>(texture);
    if (!glTexture) return;

    GLuint texID = glTexture->getTextureID();

    float x = dstRect->x;
    float y = dstRect->y;
    float w = dstRect->w;
    float h = dstRect->h;

    glBindTexture(GL_TEXTURE_2D, texID);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(x + w / 2, y + h / 2, 0);
    glRotatef(static_cast<float>(angle), 0.0f, 0.0f, 1.0f);
    glTranslatef(-w / 2, -h / 2, 0);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(1, 0); glVertex2f(w, 0);
        glTexCoord2f(1, 1); glVertex2f(w, h);
        glTexCoord2f(0, 1); glVertex2f(0, h);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

std::shared_ptr<ITexture> OpenGLRenderer::loadTextureFromFile(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        SDL_Log("Failed to load image: %s | %s", path.c_str(), IMG_GetError());
        return nullptr;
    }

    // SDL_Surface is usually in BGRA or ARGB depending on platform
    // Convert it to RGBA8888 (32-bit RGBA)
    SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!formatted) {
        SDL_Log("Failed to convert surface to RGBA32");
        return nullptr;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        formatted->w,
        formatted->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        formatted->pixels
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_DestroySurface(formatted);

    return std::make_shared<OpenGLTexture>(textureID, formatted->w, formatted->h);
}

std::shared_ptr<ITexture> OpenGLRenderer::createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        SDL_Log("TTF_RenderUTF8_Blended failed: %s", TTF_GetError());
        return nullptr;
    }

    SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!formatted) return nullptr;

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formatted->w, formatted->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, formatted->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto texture = std::make_shared<OpenGLTexture>(texID, formatted->w, formatted->h);
    SDL_DestroySurface(formatted);
    return texture;
}

}
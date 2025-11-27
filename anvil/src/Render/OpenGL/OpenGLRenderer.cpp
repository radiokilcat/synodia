#include "OpenGLRenderer.hpp"
#include "OpenGLTexture.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include <array>
#include <cmath>
#include <utility>

namespace anvil {

OpenGLRenderer::OpenGLRenderer() = default;

OpenGLRenderer::~OpenGLRenderer() {
    if (m_window && m_glContext) {
        SDL_GL_MakeCurrent(m_window, m_glContext);
    }
    destroyGLResources();
    if (m_glContext) {
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
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

    if (SDL_GL_MakeCurrent(m_window, m_glContext) != 0) {
        SDL_Log("Failed to make OpenGL context current: %s", SDL_GetError());
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
        return false;
    }

    SDL_GL_SetSwapInterval(1); // VSync

    if (!initGL(width, height)) {
        destroyGLResources();
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
        return false;
    }

    return true;
}

void OpenGLRenderer::setup2DProjection(int width, int height) {
    if (width <= 0 || height <= 0 || m_shaderProgram == 0) {
        return;
    }

    m_logicalWidth = width;
    m_logicalHeight = height;
    m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    glViewport(0, 0, width, height);
    glUseProgram(m_shaderProgram);
    if (m_projectionLoc >= 0) {
        glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(m_projection));
    }
    glUseProgram(0);
}

void OpenGLRenderer::clear() {
    glClearColor(30.0f / 255.0f, 30.0f / 255.0f, 46.0f / 255.0f, 1.0f);
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
    setup2DProjection(width, height);
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
    if (!glTexture || !dstRect || m_shaderProgram == 0) {
        return;
    }

    const float texWidth = static_cast<float>(glTexture->getWidth());
    const float texHeight = static_cast<float>(glTexture->getHeight());
    if (texWidth <= 0.0f || texHeight <= 0.0f) {
        return;
    }

    SDL_FRect effectiveSrc{};
    if (srcRect) {
        effectiveSrc = *srcRect;
    } else {
        effectiveSrc = {0.0f, 0.0f, texWidth, texHeight};
    }

    const float srcW = (effectiveSrc.w > 0.0f) ? effectiveSrc.w : texWidth;
    const float srcH = (effectiveSrc.h > 0.0f) ? effectiveSrc.h : texHeight;

    float u0 = effectiveSrc.x / texWidth;
    float v0 = effectiveSrc.y / texHeight;
    float u1 = (effectiveSrc.x + srcW) / texWidth;
    float v1 = (effectiveSrc.y + srcH) / texHeight;

    // Convert from SDL's top-left origin to OpenGL's bottom-left origin.
    float vTop = 1.0f - v0;
    float vBottom = 1.0f - v1;

    if (flip & SDL_FLIP_HORIZONTAL) {
        std::swap(u0, u1);
    }
    if (flip & SDL_FLIP_VERTICAL) {
        std::swap(vTop, vBottom);
    }

    const float x = dstRect->x;
    const float y = dstRect->y;
    const float w = dstRect->w;
    const float h = dstRect->h;

    const float centerX = center ? center->x : w * 0.5f;
    const float centerY = center ? center->y : h * 0.5f;
    const glm::vec2 rotationCenter = {x + centerX, y + centerY};

    std::array<glm::vec2, 4> corners = {
        glm::vec2{x, y},
        glm::vec2{x + w, y},
        glm::vec2{x + w, y + h},
        glm::vec2{x, y + h}
    };

    const float radians = -glm::radians(static_cast<float>(angle));
    const float cosA = std::cos(radians);
    const float sinA = std::sin(radians);

    std::array<float, 16> vertices{};
    const std::array<float, 8> texCoords = {
        u0, vTop,
        u1, vTop,
        u1, vBottom,
        u0, vBottom
    };

    for (size_t i = 0; i < corners.size(); ++i) {
        const glm::vec2 local = corners[i] - rotationCenter;
        const float rotatedX = local.x * cosA - local.y * sinA;
        const float rotatedY = local.x * sinA + local.y * cosA;
        const glm::vec2 finalPos = rotationCenter + glm::vec2(rotatedX, rotatedY);

        vertices[i * 4 + 0] = finalPos.x;
        vertices[i * 4 + 1] = finalPos.y;
        vertices[i * 4 + 2] = texCoords[i * 2 + 0];
        vertices[i * 4 + 3] = texCoords[i * 2 + 1];
    }

    glUseProgram(m_shaderProgram);
    if (m_projectionLoc >= 0) {
        glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(m_projection));
    }
    if (m_colorLoc >= 0) {
        glUniform4f(m_colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    if (m_useTextureLoc >= 0) {
        glUniform1i(m_useTextureLoc, 1);
    }
    if (m_samplerLoc >= 0) {
        glUniform1i(m_samplerLoc, 0);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture->getTextureID());

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
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

    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    if (textureID == 0) {
        SDL_Log("Failed to generate OpenGL texture for %s", path.c_str());
        SDL_DestroySurface(formatted);
        return nullptr;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    const int texWidth = formatted->w;
    const int texHeight = formatted->h;
    SDL_DestroySurface(formatted);

    return std::make_shared<OpenGLTexture>(textureID, texWidth, texHeight);
}

std::shared_ptr<ITexture> OpenGLRenderer::createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        SDL_Log("TTF_RenderUTF8_Blended failed: %s", TTF_GetError());
        return nullptr;
    }

    SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!formatted) {
        SDL_Log("Failed to convert text surface to RGBA32");
        return nullptr;
    }

    GLuint texID = 0;
    glGenTextures(1, &texID);
    if (texID == 0) {
        SDL_Log("Failed to generate OpenGL texture for text");
        SDL_DestroySurface(formatted);
        return nullptr;
    }

    glBindTexture(GL_TEXTURE_2D, texID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formatted->w, formatted->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, formatted->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    const int texWidth = formatted->w;
    const int texHeight = formatted->h;
    SDL_DestroySurface(formatted);

    return std::make_shared<OpenGLTexture>(texID, texWidth, texHeight);
}

void OpenGLRenderer::fillRect(const SDL_FRect &rect, SDL_Color color) {
    if (m_shaderProgram == 0 || rect.w == 0.0f || rect.h == 0.0f) {
        return;
    }

    std::array<float, 16> vertices = {
        rect.x,         rect.y,         0.0f, 0.0f,
        rect.x + rect.w, rect.y,         0.0f, 0.0f,
        rect.x + rect.w, rect.y + rect.h, 0.0f, 0.0f,
        rect.x,         rect.y + rect.h, 0.0f, 0.0f
    };

    glUseProgram(m_shaderProgram);
    if (m_projectionLoc >= 0) {
        glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(m_projection));
    }
    if (m_colorLoc >= 0) {
        glUniform4f(m_colorLoc,
            color.r / 255.0f,
            color.g / 255.0f,
            color.b / 255.0f,
            color.a / 255.0f);
    }
    if (m_useTextureLoc >= 0) {
        glUniform1i(m_useTextureLoc, 0);
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

bool OpenGLRenderer::initGL(int width, int height) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!initGLResources()) {
        return false;
    }

    setup2DProjection(width, height);
    return true;
}

bool OpenGLRenderer::initGLResources() {
    if (!createShaderProgram()) {
        return false;
    }
    if (!createQuadBuffers()) {
        return false;
    }
    return true;
}

bool OpenGLRenderer::createShaderProgram() {
    static const char* vertexSrc = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;

out vec2 vTexCoord;

void main() {
    vTexCoord = aTexCoord;
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}
)";

    static const char* fragmentSrc = R"(#version 330 core
in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform int uUseTexture;

out vec4 FragColor;

void main() {
    vec4 texColor = uUseTexture == 1 ? texture(uTexture, vTexCoord) : vec4(1.0);
    FragColor = texColor * uColor;
}
)";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        GLint logLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<size_t>(logLength), '\0');
        glGetShaderInfoLog(vertexShader, logLength, nullptr, log.data());
        SDL_Log("OpenGLRenderer vertex shader error: %s", log.c_str());
        glDeleteShader(vertexShader);
        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        GLint logLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<size_t>(logLength), '\0');
        glGetShaderInfoLog(fragmentShader, logLength, nullptr, log.data());
        SDL_Log("OpenGLRenderer fragment shader error: %s", log.c_str());
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        GLint logLength = 0;
        glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(static_cast<size_t>(logLength), '\0');
        glGetProgramInfoLog(m_shaderProgram, logLength, nullptr, log.data());
        SDL_Log("OpenGLRenderer shader program link error: %s", log.c_str());
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
        return false;
    }

    m_projectionLoc = glGetUniformLocation(m_shaderProgram, "uProjection");
    m_colorLoc = glGetUniformLocation(m_shaderProgram, "uColor");
    m_useTextureLoc = glGetUniformLocation(m_shaderProgram, "uUseTexture");
    m_samplerLoc = glGetUniformLocation(m_shaderProgram, "uTexture");

    glUseProgram(m_shaderProgram);
    if (m_samplerLoc >= 0) {
        glUniform1i(m_samplerLoc, 0);
    }
    glUseProgram(0);

    return true;
}

bool OpenGLRenderer::createQuadBuffers() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    if (m_vao == 0 || m_vbo == 0 || m_ebo == 0) {
        SDL_Log("OpenGLRenderer failed to create buffers");
        return false;
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    const unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 2));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void OpenGLRenderer::destroyGLResources() {
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_ebo != 0) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_shaderProgram != 0) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    m_projectionLoc = -1;
    m_colorLoc = -1;
    m_useTextureLoc = -1;
    m_samplerLoc = -1;
    m_projection = glm::mat4(1.0f);
}
}

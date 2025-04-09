#pragma once

#include "../ITexture.hpp"
#include <glad/glad.h>

namespace anvil {

class OpenGLTexture : public ITexture {
public:
    OpenGLTexture(GLuint id, int width, int height);
    ~OpenGLTexture();

    GLuint getTextureID() const;

    int getWidth() const override;
    int getHeight() const override;

private:
    GLuint textureID;
    int width;
    int height;
};

}
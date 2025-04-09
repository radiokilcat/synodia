#include "OpenGLTexture.hpp"

namespace anvil {

OpenGLTexture::OpenGLTexture(GLuint id, int w, int h)
    : textureID(id), width(w), height(h) {}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &textureID);
}

GLuint OpenGLTexture::getTextureID() const {
    return textureID;
}

int OpenGLTexture::getWidth() const {
    return width;
}

int OpenGLTexture::getHeight() const {
    return height;
}

}
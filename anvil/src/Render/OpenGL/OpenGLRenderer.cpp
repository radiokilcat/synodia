#include "OpenGLRenderer.hpp"

namespace anvil {

bool OpenGLRenderer::init(void* window, int width, int height) {
    return true;

};

void OpenGLRenderer::clear() {

};

void OpenGLRenderer::present() {

};

void OpenGLRenderer::drawTexture(/* your params */) {

};

void OpenGLRenderer::setLogicalSize(int width, int height) {

};

std::shared_ptr<ITexture> OpenGLRenderer::loadTextureFromFile(const std::string& path) {
    return {};
}

}
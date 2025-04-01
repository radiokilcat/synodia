#pragma once

#include <memory>
#include <string>
#include "ITexture.hpp"

namespace anvil {

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual bool init(void* window, int width, int height) = 0;
    virtual void clear() = 0;
    virtual void present() = 0;
    virtual void drawTexture() = 0;
    virtual void setLogicalSize(int width, int height) = 0;
    virtual std::shared_ptr<ITexture> loadTextureFromFile(const std::string& path) = 0;

};

}
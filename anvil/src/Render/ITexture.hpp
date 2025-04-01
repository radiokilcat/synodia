#pragma once

namespace anvil {

class ITexture {
public:
    virtual ~ITexture() = default;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    // optionally, expose native handle (e.g., SDL_Texture* or GLuint)
};
    
}
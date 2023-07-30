#pragma once

#include <string>
#include <map>
#include <SDL3/SDL.h>
#include <SDL_ttf.h>

namespace anvil {

class TextureManager
{
public:
    static TextureManager* instance();
    bool loadTexture(std::string filename, std::string id, SDL_Renderer* renderer);
    /* Draw the texture of given width and height */
    void draw(std::string id, int x, int y, int width, int height,
              SDL_Renderer* renderer);

    /* Draw certain frame from the spreadsheet */
    void drawFrame(std::string id, int x, int y, int width, int height,
                   int currentRow, int currentFrame,
                   SDL_Renderer* renderer);

    void drawFrameScaled(std::string id, float scale, int x, int y, int width, int height,
        int currentRow, int currentFrame,
        SDL_Renderer* renderer);

    void drawText(std::string id, std::string text,
                  TTF_Font* font, SDL_Color color,
                  int x, int y, int width, int height,
                  SDL_Renderer* renderer);

    void drawTextWrapped(std::string id, Uint32 wrapLength, std::string text,
        TTF_Font* font, SDL_Color color,
        int x, int y, int width, int height,
        SDL_Renderer* renderer);

    //TODO: also need to write draw & drawFrame methods for flipped image
    void clearFromTextureMap(std::string id);

    std::pair<int, int> textureSize(std::string id);

private:
    TextureManager();
    std::map<std::string, SDL_Texture*> m_texture_map;
};

}

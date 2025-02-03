#pragma once

#include <string>
#include <map>
#include <SDL3/SDL.h>
#include <SDL_ttf.h>
#include "vector2d.h"

namespace anvil {

class TextureManager
{
public:
    static TextureManager* instance();
    bool loadTexture(std::string filename, std::string id, SDL_Renderer* renderer);
    /* Draw the texture of given width and height */
    void draw(std::string id, float x, float y, float width, float height,
              SDL_Renderer* renderer);

    /* Draw certain frame from the spreadsheet */
    void drawFrame(std::string id, float x, float y, float width, float height,
                   int currentRow, int currentFrame, SDL_Renderer* renderer);

    void drawFrameScaled(std::string id, float scale, float x, float y, float width, float height,
        int currentRow, int currentFrame,
        SDL_Renderer* renderer);

    void drawText(std::string text, TTF_Font* font, SDL_Color color,
                  float x, float y, float width, float height,
                  SDL_Renderer* renderer);

    void drawTextWrapped(Uint32 wrapLength, std::string text, TTF_Font* font, SDL_Color color,
        float x, float y, float width, float height, SDL_Renderer* renderer);

    void drawTextMultiline(SDL_Renderer* renderer,
        TTF_Font* font,
        std::vector<std::string>& m_lines,
        SDL_Color color,
        int startLine,
        int lineCount,
        int lineHeight,
        int padding,
        Vector2D windowPosition,
        int windowWidth,
        int windowHeight,
        SDL_Color backgroundColor);

    //TODO: also need to write draw & drawFrame methods for flipped image
    void clearFromTextureMap(std::string id);

    std::pair<int, int> textureSize(std::string id);

    void drawQuadrilateral(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    void drawPoint(SDL_Renderer *renderer, float x1, float y1);
    void drawLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2);
    void drawRect(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Color color);
private:
    TextureManager();
    std::map<std::string, SDL_Texture*> m_texture_map;
};

}

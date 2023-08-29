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

    void drawTextMultiline(SDL_Renderer* renderer,
        TTF_Font* font,
        std::vector<std::string> &m_lines,
        SDL_Color color,
        Vector2D position,
        int startLine,
        int lineCount,
        int lineHeight,
        int padding,
        int width,
        int height,
        SDL_Color background);

    //TODO: also need to write draw & drawFrame methods for flipped image
    void clearFromTextureMap(std::string id);

    std::pair<int, int> textureSize(std::string id);

    void drawQuadrilateral(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    void drawPoint(SDL_Renderer *renderer, int x1, int y1);
    void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
    void drawRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color);
private:
    TextureManager();
    std::map<std::string, SDL_Texture*> m_texture_map;
};

}

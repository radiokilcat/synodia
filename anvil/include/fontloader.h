#pragma once

#include <string>
#include <map>
#include <filesystem>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>
#include <renderer.h>

namespace anvil {

class FontLoader
{
public:
    static FontLoader* instance();

    void loadFont(const std::string fontName, std::shared_ptr<Renderer> renderer, const  const std::filesystem::path path, int size);
    void closeFont(const std::string& fontName);

    FC_Font* getFont(const std::string& fontName);

    FC_Font* setDefaultFont(const std::string& fontName);
    FC_Font* getDefaultFont();

    ~FontLoader();

private:
    FontLoader();
    std::map<std::string, FC_Font*> fontsMap_;
    std::string defaultFontName_;
};

}

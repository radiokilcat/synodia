#pragma once

#include <string>
#include <map>
#include <filesystem>
#include <SDL_ttf.h>

namespace anvil {

class FontLoader
{
public:
    static FontLoader* instance();

    void loadFont(const std::string fontName, const std::filesystem::path path, int size);
    void closeFont(const std::string& fontName);

    TTF_Font* getFont(const std::string& fontName);

    TTF_Font* setDefaultFont(const std::string& fontName);
    TTF_Font* getDefaultFont();

    ~FontLoader();

private:
    FontLoader();
    std::map<std::string, TTF_Font*> fontsMap_;
    std::string defaultFontName_;
};

}

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>
#include "fontloader.h"
#include <iostream>
#include <renderer.h>

static anvil::FontLoader* instance_;

namespace anvil {

FontLoader::FontLoader()
{
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        std::exit(1);
    }
}

FontLoader* FontLoader::instance()
{
    if(instance_ == 0)
    {
        instance_ = new FontLoader;
        return instance_;
    }
    return instance_;
}

void FontLoader::loadFont(const std::string fontName, std::shared_ptr<Renderer> renderer, const std::filesystem::path path, int size)
{
    FC_Font* font = FC_CreateFont();

    if (font == nullptr)
    {
        std::cout << SDL_GetError() << " " <<  path << std::endl;
        throw std::runtime_error("Font not loaded: " + fontName);
    }
    FC_LoadFont(font, renderer->getRenderer(), path.string().c_str(), size, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    fontsMap_[fontName] = font;
}

FC_Font* FontLoader::getFont(const std::string& fontName) {
    if (fontsMap_.count(fontName) > 0) {
        return fontsMap_[fontName];
    }
    throw std::runtime_error("Font not loaded: " + fontName);
}

FC_Font* FontLoader::setDefaultFont(const std::string& fontName)
{
    if (fontsMap_.count(fontName) == 0) {
        throw std::runtime_error("Can't set default font. Font not loaded: " + fontName);
    }
    defaultFontName_ = fontName;
}

FC_Font* FontLoader::getDefaultFont()
{
    return getFont(defaultFontName_);
}

void FontLoader::closeFont(const std::string& fontName) {
    if (fontsMap_.count(fontName) > 0) {
        FC_FreeFont(fontsMap_[fontName]);
        fontsMap_.erase(fontName);

        if (fontName == defaultFontName_) {
            defaultFontName_.clear();
        }
    }
}

FontLoader::~FontLoader() {
    for (auto& pair : fontsMap_) {
        FC_FreeFont(pair.second);
    }
}

}

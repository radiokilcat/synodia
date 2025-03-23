#include "./AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL_image.h>
#include <stdexcept>

namespace anvil {

static AssetStore* instance_;

AssetStore* AssetStore::instance() {
    if(instance_ == 0) {
        instance_ = new AssetStore;
        return instance_;
    }
    return instance_;
}

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore() {
    ClearAssets();
    Logger::Log("AssetStore destructor called!");
}

void AssetStore::ClearAssets() {
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
    for (auto font: fonts) {
        TTF_CloseFont(font.second);
    }
    fonts.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!texture) {
        Logger::Log("Failed to load texture: " + filePath);
        return;
    }

    textures.emplace(assetId, texture);
    Logger::Log("Texture added to the AssetStore with id " + assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    return textures[assetId];
}

void AssetStore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize) {
    TTF_Font* font = TTF_OpenFontDPI(filePath.c_str(), fontSize, 2000, 2000);
    if (font == nullptr) {
        Logger::Log("Failed to load font: " + filePath);
        throw std::runtime_error("Failed to load font: " + filePath);
    }
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font* AssetStore::GetFont(const std::string& assetId) {
    return fonts[assetId];
}

}
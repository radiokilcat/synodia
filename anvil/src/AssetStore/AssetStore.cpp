#include "./AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL_image.h>
#include <stdexcept>
#include "../Render/IRenderer.hpp"
#include "../Render/ITexture.hpp"


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
    textures.clear();

    for (auto& [_, font] : fonts) {
        TTF_CloseFont(font);
    }
    fonts.clear();
}

void AssetStore::AddTexture(std::shared_ptr<IRenderer> renderer, const std::string& assetId, const std::string& filePath) {
    if (textures.find(assetId) != textures.end()) {
        Logger::Log("AssetStore: Texture with id {} already exists", assetId.c_str());
        return;
    }

    auto texture = renderer->loadTextureFromFile(filePath);
    if (texture) {
        textures[assetId] = texture;
    } else {
        Logger::Log("AssetStore: Failed to load texture from {}", filePath.c_str());
    }
}

std::shared_ptr<ITexture> AssetStore::GetTexture(const std::string& assetId) {
    if (textures.find(assetId) != textures.end()) {
        return textures[assetId];
    }
    return nullptr;
}


void AssetStore::AddFont(const std::string& assetId, const std::string& filePath, int fontSize) {
    if (fonts.find(assetId) != fonts.end()) {
        Logger::Log("AssetStore: Font with id {} already exists", assetId.c_str());
        return;
    }
    TTF_Font* font = TTF_OpenFontDPI(filePath.c_str(), fontSize, 2000, 2000);
    if (font == nullptr) {
        Logger::Log("Failed to load font: {}", filePath);
        throw std::runtime_error("Failed to load font: " + filePath);
    }
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font* AssetStore::GetFont(const std::string& assetId) {
    if (fonts.find(assetId) != fonts.end()) {
        return fonts[assetId];
    }
    return nullptr;
}

}
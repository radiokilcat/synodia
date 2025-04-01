#pragma once

#include <map>
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include <SDL_ttf.h>

namespace anvil {

class ITexture;
class IRenderer;

class AssetStore {
private:
    std::map<std::string, std::shared_ptr<ITexture>> textures;
    std::map<std::string, TTF_Font*> fonts;
        // TODO: create a map for audio

    public:
        static AssetStore* instance();
        AssetStore();
        ~AssetStore();

        void ClearAssets();
        void AddTexture(std::shared_ptr<IRenderer> renderer, const std::string& assetId, const std::string& filePath);
        std::shared_ptr<ITexture> GetTexture(const std::string& assetId);

        void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
        TTF_Font* GetFont(const std::string& assetId);
};

}

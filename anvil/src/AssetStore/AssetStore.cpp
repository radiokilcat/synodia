#include "./AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdexcept>
#include "../Render/IRenderer.hpp"
#include "../Render/ITexture.hpp"
#include "../Render/IFont.hpp"
#include "../Render/SDL/SDLFont.hpp"


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
	fonts.clear(); // SDLFont destructor calls TTF_CloseFont
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
	TTF_Font* raw = TTF_OpenFont(filePath.c_str(), fontSize);
	if (!raw) {
		Logger::Log("Failed to load font: {}", filePath);
		throw std::runtime_error("Failed to load font: " + filePath);
	}
	fonts.emplace(assetId, std::make_shared<SDLFont>(raw, fontSize));
}

std::shared_ptr<IFont> AssetStore::GetFont(const std::string& assetId) {
	if (fonts.find(assetId) != fonts.end()) {
		return fonts[assetId];
	}
	return nullptr;
}

}

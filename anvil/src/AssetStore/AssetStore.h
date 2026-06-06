#pragma once

#include <map>
#include <memory>
#include <string>
#include "../Render/RenderTypes.hpp"

namespace anvil {

class ITexture;
class IRenderer;
class IFont;

class AssetStore {
private:
	std::map<std::string, std::shared_ptr<ITexture>> textures;
	std::map<std::string, std::shared_ptr<IFont>> fonts;

public:
	static AssetStore* instance();
	AssetStore();
	~AssetStore();

	void ClearAssets();
	void AddTexture(std::shared_ptr<IRenderer> renderer, const std::string& assetId, const std::string& filePath);
	void AddTextureFromDecodedImage(std::shared_ptr<IRenderer> renderer, const std::string& assetId, const DecodedImage& image);
	std::shared_ptr<ITexture> GetTexture(const std::string& assetId);

	void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
	std::shared_ptr<IFont> GetFont(const std::string& assetId);
};

}

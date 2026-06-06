#pragma once

#include <memory>
#include <string>
#include "ITexture.hpp"
#include "IFont.hpp"
#include "RenderTypes.hpp"

namespace anvil {

enum class RendererType {
	SDL,
	OpenGL
};

class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual bool init(void* window, int width, int height) = 0;
	virtual void clear() = 0;
	virtual void present() = 0;
	virtual void drawTexture() = 0;
	virtual void setLogicalSize(int width, int height) = 0;
	virtual void renderTextureRotated(
		ITexture* texture,
		const FRect* srcRect,
		const FRect* dstRect,
		double angle,
		const FPoint* center,
		FlipMode flip) = 0;
	virtual std::shared_ptr<ITexture> loadTextureFromFile(const std::string& path) = 0;
	virtual DecodedImage decodeImageFromFile(const std::string& path) = 0;
	virtual std::shared_ptr<ITexture> createTextureFromDecodedImage(const DecodedImage& image) = 0;
	virtual std::shared_ptr<ITexture> createTextTexture(const std::string& text, IFont* font, Color color) = 0;
	virtual void fillRect(const FRect& rect, Color color) = 0;
	virtual RendererType getType() const = 0;
};

}

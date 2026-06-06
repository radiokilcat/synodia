#pragma once

#include "../IRenderer.hpp"

#include <SDL3/SDL.h>

namespace anvil {

class SDLRenderer : public IRenderer {
public:
	bool init(void* window, int width, int height) override;
	void clear() override;
	void present() override;
	void drawTexture() override;
	SDL_Renderer* getRawRenderer() const;
	void renderTextureRotated(
		ITexture* texture,
		const FRect* srcRect,
		const FRect* dstRect,
		double angle,
		const FPoint* center,
		FlipMode flip) override;
	std::shared_ptr<ITexture> loadTextureFromFile(const std::string& filePath) override;
	DecodedImage decodeImageFromFile(const std::string& path) override;
	std::shared_ptr<ITexture> createTextureFromDecodedImage(const DecodedImage& image) override;
	std::shared_ptr<ITexture> createTextTexture(const std::string& text, IFont* font, Color color) override;
	void setLogicalSize(int width, int height) override;
	void fillRect(const FRect& rect, Color color) override;
	void DrawGreenOutlineRect(const FRect& rect, Color color);
	RendererType getType() const override { return RendererType::SDL; }

private:
	SDL_Renderer* m_renderer = nullptr;
};

}

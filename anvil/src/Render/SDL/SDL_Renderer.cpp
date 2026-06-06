#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "SDL_Renderer.hpp"
#include "SDL_Texture.hpp"
#include "SDLFont.hpp"

#include "../../Logger/Logger.h"

namespace anvil {

// ---------------------------------------------------------------------------
// Conversion helpers — SDL types never leave this translation unit
// ---------------------------------------------------------------------------

static SDL_FRect toSDL(const FRect& r)  { return {r.x, r.y, r.w, r.h}; }
static SDL_FPoint toSDL(const FPoint& p) { return {p.x, p.y}; }
static SDL_Color  toSDL(const Color& c)  { return {c.r, c.g, c.b, c.a}; }
static SDL_FlipMode toSDL(FlipMode f)   { return static_cast<SDL_FlipMode>(static_cast<uint32_t>(f)); }

// ---------------------------------------------------------------------------

bool SDLRenderer::init(void* windowPtr, int width, int height) {
	SDL_Window* window = static_cast<SDL_Window*>(windowPtr);

	m_renderer = SDL_CreateRenderer(window, NULL);
	if (!m_renderer) {
		Logger::Err("Failed to create SDL Renderer: {}", SDL_GetError());
		return false;
	}

	if (SDL_SetRenderVSync(m_renderer, 1) != 0) {
		Logger::Err("Warning: VSync could not be enabled! SDL_Error: ", SDL_GetError());
	}

	if (SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND) != 0) {
		Logger::Err("Warning: Failed to set blend mode! SDL_Error: ", SDL_GetError());
	}
	SDL_SetRenderDrawColor(m_renderer, 30, 30, 46, SDL_ALPHA_OPAQUE);
	return true;
}

void SDLRenderer::clear() {
	SDL_RenderClear(m_renderer);
}

void SDLRenderer::present() {
	SDL_RenderPresent(m_renderer);
}

void SDLRenderer::setLogicalSize(int width, int height) {
	if (!SDL_SetRenderLogicalPresentation(m_renderer, width, height,
		SDL_LOGICAL_PRESENTATION_STRETCH)) {
		Logger::Err("Failed to set logical presentation: ", SDL_GetError());
	}
}

void SDLRenderer::fillRect(const FRect& rect, Color color) {
	Uint8 originalR, originalG, originalB, originalA;
	SDL_GetRenderDrawColor(m_renderer, &originalR, &originalG, &originalB, &originalA);

	SDL_Color c = toSDL(color);
	SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
	SDL_FRect r = toSDL(rect);
	SDL_RenderFillRect(m_renderer, &r);

	SDL_SetRenderDrawColor(m_renderer, originalR, originalG, originalB, originalA);
}

void SDLRenderer::DrawGreenOutlineRect(const FRect& rect, Color color) {
	Uint8 originalR, originalG, originalB, originalA;
	SDL_GetRenderDrawColor(m_renderer, &originalR, &originalG, &originalB, &originalA);

	SDL_Color c = toSDL(color);
	SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
	SDL_FRect r = toSDL(rect);
	SDL_RenderRect(m_renderer, &r);

	SDL_SetRenderDrawColor(m_renderer, originalR, originalG, originalB, originalA);
}

void SDLRenderer::drawTexture() {}

SDL_Renderer* SDLRenderer::getRawRenderer() const {
	return m_renderer;
}

void SDLRenderer::renderTextureRotated(
	ITexture* texture,
	const FRect* srcRect,
	const FRect* dstRect,
	double angle,
	const FPoint* center,
	FlipMode flip)
{
	SDLTexture* sdlTex = dynamic_cast<SDLTexture*>(texture);
	if (!sdlTex || !m_renderer) {
		SDL_Log("SDLRenderer::renderTextureRotated: Invalid texture or renderer");
		return;
	}

	SDL_FRect sdlSrc, sdlDst;
	const SDL_FRect* pSrc = nullptr;
	const SDL_FRect* pDst = nullptr;
	if (srcRect) { sdlSrc = toSDL(*srcRect); pSrc = &sdlSrc; }
	if (dstRect) { sdlDst = toSDL(*dstRect); pDst = &sdlDst; }

	SDL_FPoint sdlCenter;
	const SDL_FPoint* pCenter = nullptr;
	if (center) { sdlCenter = toSDL(*center); pCenter = &sdlCenter; }

	SDL_RenderTextureRotated(
		m_renderer,
		sdlTex->getSDLTexture(),
		pSrc,
		pDst,
		angle,
		pCenter,
		toSDL(flip)
	);
}

std::shared_ptr<ITexture> SDLRenderer::createTextureFromDecodedImage(const DecodedImage& image) {
	if (!image.isValid()) {
		SDL_Log("SDLRenderer::createTextureFromDecodedImage: invalid DecodedImage (%dx%d, %zu bytes)",
			image.width, image.height, image.pixels.size());
		return nullptr;
	}
	SDL_Surface* surface = SDL_CreateSurfaceFrom(
		image.width, image.height, SDL_PIXELFORMAT_RGBA32,
		const_cast<void*>(static_cast<const void*>(image.pixels.data())),
		image.width * 4);
	if (!surface) {
		SDL_Log("SDLRenderer::createTextureFromDecodedImage: SDL_CreateSurfaceFrom failed: %s", SDL_GetError());
		return nullptr;
	}
	SDL_Texture* sdlTex = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_DestroySurface(surface);
	if (!sdlTex) {
		SDL_Log("SDLRenderer::createTextureFromDecodedImage: SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
		return nullptr;
	}
	return std::make_shared<SDLTexture>(sdlTex);
}

DecodedImage SDLRenderer::decodeImageFromFile(const std::string& path) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface) {
		SDL_Log("SDLRenderer::decodeImageFromFile: IMG_Load failed: %s", SDL_GetError());
		return {};
	}
	SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
	SDL_DestroySurface(surface);
	if (!converted) {
		SDL_Log("SDLRenderer::decodeImageFromFile: SDL_ConvertSurface failed: %s", SDL_GetError());
		return {};
	}
	DecodedImage img;
	img.width = converted->w;
	img.height = converted->h;
	const int rowBytes = converted->w * 4;
	img.pixels.resize(static_cast<size_t>(converted->h) * rowBytes);
	const uint8_t* src = static_cast<const uint8_t*>(converted->pixels);
	for (int y = 0; y < converted->h; ++y)
		SDL_memcpy(img.pixels.data() + y * rowBytes, src + y * converted->pitch, rowBytes);
	SDL_DestroySurface(converted);
	return img;
}

std::shared_ptr<ITexture> SDLRenderer::loadTextureFromFile(const std::string& filePath) {
	DecodedImage img = decodeImageFromFile(filePath);
	if (img.pixels.empty()) return nullptr;
	return createTextureFromDecodedImage(img);
}

std::shared_ptr<ITexture> SDLRenderer::createTextTexture(const std::string& text, IFont* font, Color color) {
	auto* sdlFont = dynamic_cast<SDLFont*>(font);
	if (!sdlFont) {
		Logger::Err("SDLRenderer::createTextTexture: invalid font");
		return nullptr;
	}

	SDL_Color sdlColor = toSDL(color);
	SDL_Surface* surface = TTF_RenderText_Blended(sdlFont->getRawFont(), text.c_str(), 0, sdlColor);
	if (!surface) {
		Logger::Log("TTF_RenderText_Blended failed: {}", SDL_GetError());
		return nullptr;
	}

	SDL_Texture* sdlTex = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_DestroySurface(surface);

	if (!sdlTex) {
		Logger::Log("Failed to create SDL_Texture from surface");
		return nullptr;
	}

	return std::make_shared<SDLTexture>(sdlTex);
}

}

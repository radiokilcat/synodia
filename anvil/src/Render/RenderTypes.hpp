#pragma once

#include <cstdint>
#include <vector>

namespace anvil {

struct FRect {
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
};

struct FPoint {
	float x = 0.0f;
	float y = 0.0f;
};

struct Color {
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 255;
};

// Values intentionally match SDL3's SDL_FLIP_* constants for zero-cost cast in SDLRenderer.
enum class FlipMode : uint32_t {
	None       = 0,
	Horizontal = 1,
	Vertical   = 2
};

inline FlipMode operator|(FlipMode a, FlipMode b) {
	return static_cast<FlipMode>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline bool operator&(FlipMode a, FlipMode b) {
	return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
}

struct DecodedImage {
	int width = 0;
	int height = 0;
	std::vector<uint8_t> pixels; // RGBA8, row-major, tightly packed

	bool isValid() const {
		return width > 0 && height > 0 &&
		       pixels.size() == static_cast<size_t>(width) * height * 4;
	}
};

} // namespace anvil

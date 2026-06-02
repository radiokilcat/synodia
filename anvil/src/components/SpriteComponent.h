#pragma once

#include <string>
#include "../Render/RenderTypes.hpp"

enum class direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	int zIndex;
	bool isFixed;
	anvil::FlipMode flip;
	anvil::FRect srcRect;
	direction dir;

	SpriteComponent(std::string assetId = "",
	                int width = 0,
	                int height = 0,
	                int zIndex = 0,
	                bool isFixed = false,
	                int srcRectX = 0,
	                int srcRectY = 0,
	                direction dir = direction::DOWN)
	{
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->zIndex = zIndex;
		this->flip = anvil::FlipMode::None;
		this->isFixed = isFixed;
		this->srcRect = {static_cast<float>(srcRectX), static_cast<float>(srcRectY),
		     static_cast<float>(width), static_cast<float>(height)};
		this->dir = dir;
	}
};

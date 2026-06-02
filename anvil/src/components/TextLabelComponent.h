#pragma once

#include <string>
#include <glm/glm.hpp>
#include "../Render/RenderTypes.hpp"

namespace anvil {

struct TextLabelComponent {
	glm::vec2 position;
	std::string text;
	std::string assetId;
	Color color;
	bool isFixed;
	bool isNested;

	TextLabelComponent(glm::vec2 position = glm::vec2(0),
	                   const std::string& text = "",
	                   const std::string& assetId = "",
	                   const Color& color = {0, 0, 0, 255},
	                   bool isNested = false,
	                   bool isFixed = true) {
		this->position = position;
		this->text = text;
		this->assetId = assetId;
		this->color = color;
		this->isFixed = isFixed;
	}
};

}

#pragma once

#include "../Render/RenderTypes.hpp"
#include "../Render/ITexture.hpp"
#include "../Commands/ICommand.hpp"

#include <string>
#include <vector>
#include <optional>

namespace anvil {

enum class State {
	DEFAULT,
	HOVER,
	CLICK
};

struct CellUIComponent {
	std::optional<std::string> defaultAsset;
	std::optional<std::string> hoverAsset;
	std::optional<std::string> clickAsset;
	FRect srcRect;
	float width;
	float height;
};

struct RowUIComponent {
	CellUIComponent left;
	CellUIComponent mid;
	CellUIComponent right;
	std::optional<std::string> callback;
	State state = State::DEFAULT;
};

}

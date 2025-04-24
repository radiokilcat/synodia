#pragma once

#include <SDL3/SDL.h>
#include "../Render/ITexture.hpp"
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
    SDL_FRect srcRect;
    float width;
    float height;
};

struct RowUIComponent {
    CellUIComponent left;
    CellUIComponent mid;
    CellUIComponent right;
    State state = State::DEFAULT;
};

}
#pragma once

#include "../IFont.hpp"
#include <SDL3_ttf/SDL_ttf.h>

namespace anvil {

class SDLFont : public IFont {
public:
	SDLFont(TTF_Font* font, int size);
	~SDLFont() override;
	int getSize() const override;
	TTF_Font* getRawFont() const;

private:
	TTF_Font* m_font = nullptr;
	int m_size = 0;
};

} // namespace anvil

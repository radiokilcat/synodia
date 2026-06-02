#include "SDLFont.hpp"
#include <SDL3_ttf/SDL_ttf.h>

namespace anvil {

SDLFont::SDLFont(TTF_Font* font, int size)
	: m_font(font), m_size(size) {}

SDLFont::~SDLFont() {
	if (m_font) {
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
}

int SDLFont::getSize() const {
	return m_size;
}

TTF_Font* SDLFont::getRawFont() const {
	return m_font;
}

} // namespace anvil

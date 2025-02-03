#pragma once
#include <SDL3/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "game_objects/gameobject.h"

namespace anvil {
	class ScrollableText : public GameObject
	{
	public:
		ScrollableText(TTF_Font* font);
		virtual void draw(std::shared_ptr<Renderer> renderer) override;
		virtual void update(Uint64 deltaTime) override;
		static bool registerWithFactory();
		void init() override {};
	private:
		TTF_Font* font_ = nullptr;
		SDL_Color White = { 255, 255, 255 };
		std::vector<std::string> m_lines;
		int lineCount = 3;
		int lineHeight = 48;
		int padding = 16;
		int startLine = 0;
		Uint64 lastUpdate = 0;
 	};
}
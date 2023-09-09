#pragma once
#include <SDL3/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "SDL_FontCache.h"
#include "game_objects/gameobject.h"

namespace anvil {
	class ScrollableText : public GameObject
	{
	public:
		ScrollableText(FC_Font* font);
		virtual void draw(std::shared_ptr<Renderer> renderer) override;
		virtual void update() override;
		static bool registerWithFactory();
	private:
		FC_Font* font_ = nullptr;
		SDL_Color White = { 255, 255, 255 };
		std::vector<std::string> m_lines;
		int lineCount = 3;
		int lineHeight = 48;
		int padding = 16;
		int startLine = 0;
		float scrollOffset = 0;
		Uint64 lastUpdate = 0;

		Vector2D size{ 600.f, (float)200.f };
		Vector2D contentOffset{ (float)0.f, (float)0.f };
		Vector2D contentSize{ (float)600.f, (float)350.f };
 	};
}
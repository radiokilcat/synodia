#include "ui/scrollable_text.h"
#include "game_objects/GameObjectsFactory.h"
#include "game_objects/gameobject.h"
#include "fontloader.h"
#include <iostream>
#include "SDL_FontCache.h"

namespace anvil {
	ScrollableText::ScrollableText(FC_Font* font = FontLoader::instance()->getDefaultFont()) : GameObject() {
		std::ifstream in("res/tale.txt", std::ios_base::in); // the in flag is optional
		std::string line;
		while (std::getline(in, line))
		{
			m_lines.push_back(line);
		}
		font_ = font;
	}

	void ScrollableText::draw(std::shared_ptr<Renderer> renderer)
	{
		TextureManager::instance()->drawTextMultilineScroll(
			renderer->getRenderer(),
			position_,
			size,
			contentOffset,
			contentSize,
			White,
			{ 34, 39, 46 },
			font_,
			m_lines,
			startLine,
			lineCount,
			lineHeight,
			padding
		);
		 
	}

	void ScrollableText::update() {
		const auto inputHandler = anvil::InputHandler::instance();
		scrollOffset += inputHandler->getMouseWheelOffset()->y();
		
		auto shownAt = anvil::Application::Instance()->getTicks();
		contentOffset.setY(contentOffset.y() + 3 * inputHandler->getMouseWheelOffset()->y());
		if (contentOffset.y() > 0 && contentOffset.y() > lineHeight) {
			startLine -= contentOffset.y() / lineHeight;
			if (startLine < 0) {
				startLine = 0;
			}
			contentOffset.setY(0);
		}
		else if (contentOffset.y() < 0 && (-1 * contentOffset.y()) > lineHeight) {
			startLine += -1 * ( contentOffset.y() / lineHeight);
			if (startLine >= m_lines.size()) {
				startLine = 0;
			}
			contentOffset.setY(0);
		}
		lastUpdate = shownAt;

		return;
		if (scrollOffset != 0) {
			const auto mousePosition = inputHandler->getMousePosition();
			const auto isMouseInside = mousePosition->x() >= position_.x()
				&& mousePosition->x() <= position_.x() + width_
				&& mousePosition->y() >= position_.y()
				&& mousePosition->y() <= position_.y() + height_;
			if (isMouseInside && scrollOffset < 0) {
				startLine += 1;
			} 
			if (isMouseInside && scrollOffset > 0) {
				startLine -= 1;
			}
			if (startLine > m_lines.size() || startLine < 0) {
				startLine = 0;
			}
			scrollOffset = 0;
		}
	}
	bool ScrollableText::registerWithFactory() {
		GameObjectFactory::instance().registerType("ScrollableText", []() -> std::unique_ptr<BaseGameObject> {
			return std::make_unique<ScrollableText>();
			});
		return true;
	}
}

#include "ui/scrollable_text.h"
#include "game_objects/GameObjectsFactory.h"
#include "game_objects/gameobject.h"
#include "fontloader.h"

namespace anvil {
	ScrollableText::ScrollableText(TTF_Font* font = FontLoader::instance()->getDefaultFont()) : GameObject() {
		std::ifstream in("res/tale.txt", std::ios_base::in); // the in flag is optional
		std::string line;
		while (std::getline(in, line))
		{
			m_lines.push_back(line);
		}
		font_ = font;
	}
	void ScrollableText::draw(std::shared_ptr<Renderer> renderer) {
		TextureManager::instance()->drawTextMultiline(
			renderer->getRenderer(),
			font_,
			m_lines,
			White,
			startLine,
			lineCount,
			lineHeight,
			padding,
			position_,
			width_,
			height_,
			{ 34, 39, 46 });
		 
	}

	void ScrollableText::update() {
		const auto inputHandler = anvil::InputHandler::instance();
		auto lkmPressed = inputHandler->getMouseButtonState(0);
		auto rkmPressed = inputHandler->getMouseButtonState(2);
		auto shownAt = anvil::Application::Instance()->getTicks();
		if (shownAt - lastUpdate < 200) {
			return;
		}
		lastUpdate = shownAt;
		if (lkmPressed || rkmPressed) {
			const auto mousePosition = inputHandler->getMousePosition();
			const auto isMouseInside = mousePosition->x() >= position_.x()
				&& mousePosition->x() <= position_.x() + width_
				&& mousePosition->y() >= position_.y()
				&& mousePosition->y() <= position_.y() + height_;
			if (isMouseInside && lkmPressed) {
				startLine += lineCount;
			} 
			if (isMouseInside && rkmPressed) {
				startLine -= lineCount;
			}
			if (startLine > m_lines.size() || startLine < 0) {
				startLine = 0;
			}
		}
	}
	bool ScrollableText::registerWithFactory() {
		GameObjectFactory::instance().registerType("ScrollableText", []() -> std::unique_ptr<BaseGameObject> {
			return std::make_unique<ScrollableText>();
			});
		return true;
	}
}

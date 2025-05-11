#pragma once

#include "ImguiSystem.h"


namespace anvil {
	
	class IWidget;
	
	class MenuBar : public IWidget {
	public:
		void draw() override;
		void drawWidgets();
		void shutDown();

	private:
		bool showInspector = true;
		std::queue<std::function<void()>> callbacks;
		bool show_demo_window = false;
	};
}

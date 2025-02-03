#pragma once

#include "ImguiSystem.h"


namespace anvil {
	
	class IWidget;
	
	class MenuBar : public IWidget {
	public:
		void draw() override;
		void drawWidgets();
		void shutDown();
		void addGameObject(GameObject* obj);
		void setRootNode(std::shared_ptr<GameObject> node);
		void drawPropertiesWidget(std::shared_ptr<GameObject> node);

		void drawSingleNode(std::shared_ptr<GameObject> node);
		void drawNode(std::weak_ptr<GameObject> node);

	private:
		bool showInspector = true;
		std::vector<GameObject*> objects;
		std::queue<std::function<void()>> callbacks;
		std::shared_ptr<GameObject> scene;
		std::shared_ptr<GameObject> currentObj;
		bool show_demo_window = false;
	};
}

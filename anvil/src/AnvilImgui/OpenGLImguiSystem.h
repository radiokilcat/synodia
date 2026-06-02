#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include "window.h"
#include "ImguiSystem.h"

namespace anvil {

class OpenGLImguiSystem : public ImguiSystem {
public:
	OpenGLImguiSystem() = default;
	~OpenGLImguiSystem() = default;
	void init(SDL_Window* window, std::shared_ptr<IRenderer> renderer) override;
	void update() override;
	void render() override;
	void shutDown() override;
	void handleEvent(SDL_Event& event) override;
	void RegisterWidget(const std::string& name, std::shared_ptr<IWidget> widget) override;
	void UnregisterWidget(const std::string& name) override;
	void ShowWidget(const std::string& name) override;
	void HideWidget(const std::string& name) override;
	void addUpdateTask(std::function<void()> task) override;

private:
	bool m_initialized = false;
	std::vector<std::function<void()>> updateTasks;
	std::unordered_map<std::string, std::shared_ptr<IWidget>> widgets;
	std::unordered_map<std::string, bool> widgetVisibility;
};

} // namespace anvil

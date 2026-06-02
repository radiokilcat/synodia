#include "OpenGLImguiSystem.h"
#include "MenuBar.h"
#include "SceneWidget.h"
#include <imgui.h>
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

namespace anvil {

void OpenGLImguiSystem::init(SDL_Window* window, std::shared_ptr<IRenderer> renderer) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	if (!ImGui_ImplSDL3_InitForOpenGL(window, SDL_GL_GetCurrentContext())) {
		Logger::Err("OpenGLImguiSystem: ImGui_ImplSDL3_InitForOpenGL failed");
		return;
	}
	if (!ImGui_ImplOpenGL3_Init("#version 330 core")) {
		Logger::Err("OpenGLImguiSystem: ImGui_ImplOpenGL3_Init failed");
		ImGui_ImplSDL3_Shutdown();
		return;
	}
	m_initialized = true;
	RegisterWidget("MenuBar", std::make_shared<MenuBar>());
}

void OpenGLImguiSystem::render() {
	if (!m_initialized) return;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	for (const auto& pair : widgets) {
		if (widgetVisibility[pair.first]) {
			pair.second->draw();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGLImguiSystem::handleEvent(SDL_Event& event) {
	ImGui_ImplSDL3_ProcessEvent(&event);
}

void OpenGLImguiSystem::update() {
	for (const auto& task : updateTasks) {
		task();
	}
	updateTasks.clear();
}

void OpenGLImguiSystem::shutDown() {
	if (m_initialized) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
	}
	ImGui::DestroyContext();
}

void OpenGLImguiSystem::RegisterWidget(const std::string& name, std::shared_ptr<IWidget> widget) {
	widgets[name] = widget;
	widgetVisibility[name] = false;
}

void OpenGLImguiSystem::UnregisterWidget(const std::string& name) {
	widgets.erase(name);
	widgetVisibility.erase(name);
}

void OpenGLImguiSystem::ShowWidget(const std::string& name) {
	if (widgetVisibility.find(name) != widgetVisibility.end()) {
		widgetVisibility[name] = true;
	}
}

void OpenGLImguiSystem::HideWidget(const std::string& name) {
	if (widgetVisibility.find(name) != widgetVisibility.end()) {
		widgetVisibility[name] = false;
	}
}

void OpenGLImguiSystem::addUpdateTask(std::function<void()> task) {
	updateTasks.push_back(task);
}

} // namespace anvil

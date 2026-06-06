#include <nlohmann/json.hpp>
#include "LoadingState.h"
#include "../Logger/Logger.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../AssetStore/AssetStore.h"
#include "../Application.h"
#include "../systems/MovementSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/AnimationSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/RenderColliderSystem.h"
#include "../systems/DamageSystem.h"
#include "../systems/KeyboardControlSystem.h"
#include "../systems/CameraMovementSystem.h"
#include "../systems/ProjectileEmitSystem.h"
#include "../systems/ProjectileLifecycleSystem.h"
#include "../systems/RenderTextSystem.h"
#include "../systems/RenderHealthBarSystem.h"
#include "../systems/RenderImGUISystem.h"
#include "../systems/ButtonSystem.h"
#include "../systems/RenderTileMapSystem.h"

namespace anvil {

LoadingState::LoadingState(std::unique_ptr<ILoadableState> state)
        : registry(std::make_unique<Registry>())
        , assetStore(std::make_unique<AssetStore>())
        , stateToLoad(std::move(state))
        , eventBus(std::make_unique<EventBus>()) {

	stateLoader = std::make_unique<StateLoader>(registry);
}

bool LoadingState::onEnter() {
	Logger::Log("Enter Loading state");

	assetsToLoad = stateToLoad->getAssetsToLoad();
	registry->AddSystem<RenderTextSystem>();

	Entity newEntity = registry->CreateEntity();
	assetStore->AddFont("vera", "assets/fonts/VeraMono.ttf", 24);
	Entity loadingText = registry->CreateEntity();
	loadingText.AddComponent<TextLabelComponent>( glm::vec2(50, 50), "Loading...",
	"vera", anvil::Color{255, 255, 255, 255}, false, true);
	loadingText.Tag("loading-text");

	Entity currentFile = registry->CreateEntity();
	currentFile.AddComponent<TextLabelComponent>( glm::vec2(50, 90), "",
	"vera", anvil::Color{255, 255, 255, 255}, false, true);
	currentFile.Tag("current-file");

	uploadedCount = 0;
	progress = 0.0f;
	totalToLoad = assetsToLoad.size();
	loadStartTime = std::chrono::steady_clock::now();

	for (const auto& req : assetsToLoad) {
		if (req.type == AssetRequest::Type::Font) {
			assetStore->AddFont(req.id, req.path, 24);
			++uploadedCount;
		} else {
			auto renderer = Application::Instance()->getRenderer();
			loadFutures.push_back(threadPool.submit([this, req, renderer]() {
				DecodedImage img = renderer->decodeImageFromFile(req.path);
				if (img.pixels.empty()) {
					Logger::Log("LoadingState: failed to decode {}", req.path);
					++uploadedCount;
					return;
				}

				queueSemaphore.acquire(); // after decode: slot held only while item waits in queue
				std::lock_guard lock(pendingMutex);
				pendingSurfaces.push({req.id, req.path, std::move(img)});
			}));
		}
	}

	return true;
}

bool LoadingState::onExit() {
	Logger::Log("Exit Loading state");
	// drain queue first: releases semaphore slots so any blocked decoders can finish
	{
		std::lock_guard lock(pendingMutex);
		while (!pendingSurfaces.empty()) {
			pendingSurfaces.pop();
			queueSemaphore.release();
		}
	}
	for (auto& f : loadFutures) {
		if (f.valid()) f.get();
	}
	// discard anything that arrived after the initial drain
	std::lock_guard lock(pendingMutex);
	while (!pendingSurfaces.empty())
		pendingSurfaces.pop();
	return true;
}

void LoadingState::update(double deltaTime) {
	registry->Update();

	if (progress >= 1.0f) {
		auto elapsed = std::chrono::steady_clock::now() - loadStartTime;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		Logger::Log("LoadingState: loaded {} assets in {} ms (async)", assetsToLoad.size(), ms);
		stateToLoad->onAssetsLoaded(std::move(*assetStore));
		auto state = stateToLoad.release();
		auto game_state = dynamic_cast<GameState*>(state);
		Application::Instance()->getStateMachine()->changeState(game_state);
		return;
	}

	// GPU upload on main thread — drain whatever background threads decoded this frame
	auto renderer = Application::Instance()->getRenderer();
	// drain queue under brief lock, GPU upload runs outside lock
	std::vector<PendingSurface> batch;
	{
		std::lock_guard lock(pendingMutex);
		while (!pendingSurfaces.empty()) {
			batch.push_back(std::move(pendingSurfaces.front()));
			pendingSurfaces.pop();
			queueSemaphore.release();
		}
	}
	std::string lastPath;
	for (auto& pending : batch) {
		assetStore->AddTextureFromDecodedImage(renderer, pending.id, pending.image);
		lastPath = pending.path;
		++uploadedCount;
	}

	if (!lastPath.empty()) {
		registry->GetEntityByTag("current-file").GetComponent<TextLabelComponent>().text = lastPath.c_str();
	}

	progress = totalToLoad > 0
		? static_cast<float>(uploadedCount.load()) / static_cast<float>(totalToLoad)
		: 1.0f;
}

void LoadingState::render(std::shared_ptr<IRenderer> renderer) {
	registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);
	anvil::FRect barBg = { 50.0f, 120.0f, 300.0f, 25.0f };
	renderer->fillRect(barBg, {255, 255, 255, 255});
	anvil::FRect barFill = barBg;
	barFill.w *= progress;
	renderer->fillRect(barFill, {100, 100, 100, 255});
}

void LoadingState::handleInput(SDL_Event& event) {
}


}

#pragma once

#include "gamestate.h"
#include "stateLoader.hpp"
#include "../EventBus/EventBus.h"
#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../Render/IRenderer.hpp"
#include "game_state_machine.h"
#include "ILoadable.hpp"
#include <chrono>
#include <future>
#include <mutex>
#include <atomic>
#include <queue>
#include <semaphore>
#include "../ThreadPool/ThreadPool.hpp"

namespace anvil {

class LoadingState : public GameState
{
public:
	LoadingState(std::unique_ptr<ILoadableState> state);
	bool onEnter() override;
	bool onExit() override;

	void update(double deltaTime) override;
	void render(std::shared_ptr<IRenderer> renderer) override;
	void handleInput(SDL_Event& event) override;
	std::string getID() { return m_id; };
	void setDebug(bool debug) override { isDebug = debug; }

	std::unique_ptr<Registry>& getRegistry() { return registry; }
	std::unique_ptr<AssetStore>& getAssetStore() { return assetStore; }

private:
	struct PendingSurface {
		std::string id;
		std::string path;
		DecodedImage image;
	};

	std::unique_ptr<Registry> registry;
	std::unique_ptr<StateLoader> stateLoader;
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;
	std::shared_ptr<ITexture> text;
	std::unique_ptr<ILoadableState> stateToLoad;
	std::vector<AssetRequest> assetsToLoad;

	std::mutex pendingMutex;
	std::queue<PendingSurface> pendingSurfaces;
	static constexpr std::ptrdiff_t kMaxQueueDepth = 32;
	std::counting_semaphore<kMaxQueueDepth> queueSemaphore{kMaxQueueDepth};
	std::vector<std::future<void>> loadFutures;
	ThreadPool threadPool{std::max(1u, std::thread::hardware_concurrency())};
	std::atomic<size_t> uploadedCount{0};
	size_t totalToLoad = 0;

	GameState* nextState;
	const std::string m_id = "load";
	float progress = 0.0f;
	bool isDebug = false;
	SDL_Rect camera;

	std::chrono::steady_clock::time_point loadStartTime;
};


}

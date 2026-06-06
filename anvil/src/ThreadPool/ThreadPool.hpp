#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace anvil {

class ThreadPool {
public:
	explicit ThreadPool(size_t threadCount) {
		for (size_t i = 0; i < threadCount; ++i) {
			workers.emplace_back([this] {
				for (;;) {
					std::function<void()> task;
					{
						std::unique_lock lock(queueMutex);
						cv.wait(lock, [this] { return stop || !tasks.empty(); });
						if (stop && tasks.empty()) return;
						task = std::move(tasks.front());
						tasks.pop();
					}
					task();
				}
			});
		}
	}

	~ThreadPool() {
		{
			std::unique_lock lock(queueMutex);
			stop = true;
		}
		cv.notify_all();
		for (auto& w : workers) w.join();
	}

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	template<class F>
	auto submit(F&& f) -> std::future<std::invoke_result_t<F>> {
		using R = std::invoke_result_t<F>;
		auto task = std::make_shared<std::packaged_task<R()>>(std::forward<F>(f));
		std::future<R> fut = task->get_future();
		{
			std::unique_lock lock(queueMutex);
			if (stop) throw std::runtime_error("ThreadPool is stopped");
			tasks.emplace([task] { (*task)(); });
		}
		cv.notify_one();
		return fut;
	}

	size_t threadCount() const { return workers.size(); }

private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex queueMutex;
	std::condition_variable cv;
	bool stop = false;
};

} // namespace anvil

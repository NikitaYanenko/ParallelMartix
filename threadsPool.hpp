#ifndef THREADS_POOL_H
#define THREADS_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <iostream>

class ThreadsPool {
private:
	const unsigned int numThreads;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> queueTasks;
	std::mutex queueMutex;
	std::condition_variable workAvailable;
	bool isStop;

	void workerThread();
public:
	ThreadsPool(const unsigned int numThreads);
	~ThreadsPool();

	[[nodiscard]] int workerThreads() const;

	template<class F, class... Args>
	[[nodiscard]] auto submit(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
		using returnType = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared<std::packaged_task<returnType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

		std::future<returnType> res = task->get_future();

		{
			std::unique_lock<std::mutex> lock(queueMutex);
			queueTasks.emplace([task]() { (*task)(); });
		}

		workAvailable.notify_one();
		
		return res;
	}
};

#endif
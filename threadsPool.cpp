#include "threadsPool.hpp"

void ThreadsPool::workerThread() {
	while (true) {
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(queueMutex);
			workAvailable.wait(lock, [this]() { return isStop || !queueTasks.empty();});
			if (isStop && queueTasks.empty()) {
				return;
			}

			task = std::move(queueTasks.front());
			queueTasks.pop();
		}

		task();
	}
}

ThreadsPool::ThreadsPool(const unsigned int numThreads) : numThreads(numThreads), isStop(false) {
	for (unsigned int i = 0; i < this->numThreads; ++i) {
		threads.emplace_back([this]() { workerThread();});
	}
}

ThreadsPool::~ThreadsPool() {
	{
		std::unique_lock<std::mutex> lock(queueMutex);
		isStop = true;
	}

	workAvailable.notify_all();

	for (std::thread& worker : threads)
		worker.join();
}

int ThreadsPool::workerThreads() const {
	return numThreads;
}
#pragma once
#include "safe_queue.h"

using namespace std::chrono_literals;

class ThreadPool {
private:
	std::mutex threadMutex;
	std::vector<std::thread> vectorOfThreads;
	safe_queue<std::function<void()>> safeQueueOfWorks;
	bool isSubmit = false;
public:
	ThreadPool(int cores) {
		for (int i = 0; i < cores; ++i) {
			vectorOfThreads.push_back(std::thread(&ThreadPool::work, this));
		}
	}
	~ThreadPool() {
		for (auto& thread : vectorOfThreads) {
			thread.join();
		}
	}

	// Выбирает из очереди очередную работу и исполняет её
	void work() {
		while (!isSubmit) {
			std::unique_lock<std::mutex> lock(threadMutex);
			if (!safeQueueOfWorks.empty()) {
				safeQueueOfWorks.front();
				safeQueueOfWorks.pop();
			}
			else {
				std::this_thread::yield();	
			}
			lock.unlock();
		}
	}

	// Помещение в очередь работы
	void submit(std::vector<std::function<void()>> vectorFunctions, int countSubmits) {
		for (int i = 0; i < countSubmits; ++i) {
			for (size_t j = 0; j < vectorFunctions.size(); ++j) {
				safeQueueOfWorks.push(vectorFunctions.at(j));
			}
			std::this_thread::sleep_for(1s);
		}
		isSubmit = true;
	}
};
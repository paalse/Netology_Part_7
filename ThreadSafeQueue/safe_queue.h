#pragma once

template<class T>
class safe_queue {
private:
	std::queue<T> queueOfWorks;
	std::mutex queueMutex;
	std::condition_variable cond_variable;
public:
	// Добавить элемент в очередь
	void push(T work) {
		std::unique_lock<std::mutex> lock(queueMutex);
		queueOfWorks.push(work);
		cond_variable.notify_one();
	}
	
	// Получение элемента из очереди
	T pop() {
		std::unique_lock<std::mutex> lock(queueMutex);
		cond_variable.wait(lock, [this]() { return !queueOfWorks.empty(); });
		T work = std::move(queueOfWorks.front());
		queueOfWorks.pop();
		return work;
	};
	
	// Получение количества элементов в очереди
	int size() {
		std::unique_lock<std::mutex> lock(queueMutex);
		return queueOfWorks.size();
	}

	// Проверка очереди на наличие элементов
	bool empty() {
		std::unique_lock<std::mutex> lock(queueMutex);
		return queueOfWorks.empty();
	}

	// Получение первого элемента очереди
	void front() {
		std::unique_lock<std::mutex> lock(queueMutex);
		queueOfWorks.front()();
	}

	// Получение последнего элемента очереди
	void back() {
		std::unique_lock<std::mutex> lock(queueMutex);
		queueOfWorks.back()();
	}
};
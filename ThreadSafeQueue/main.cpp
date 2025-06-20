#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <functional>
#include <queue>
#include <atomic>
#include "ThreadPool.h"

using namespace std::chrono_literals;

void func1() {
	std::cout << __FUNCTION__ << " (" << std::this_thread::get_id() << ")" << std::endl;
}

void func2() {
	std::cout << __FUNCTION__ << " (" << std::this_thread::get_id() << ")" << std::endl;
}

int main() {
	const int cores = std::thread::hardware_concurrency(); // Получение количества ядер
	std::vector<std::function<void()>> vectorOfFunctions{ func1, func2 };
	int countSubmits = 10;			 // Количество работ
	ThreadPool thread_pool{ cores }; // Создание пула потоков
	std::thread thread1(&ThreadPool::submit, &thread_pool, vectorOfFunctions, countSubmits); // Заполнение очереди
	thread1.join();
	return 0;
}
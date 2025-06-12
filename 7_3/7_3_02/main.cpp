//Задание 2
//For_each
//Реализуйте параллельный вариант оператора for_each.
//Функция должна принимать итераторы на начало и конец контейнера и ссылку, обрабатывающую функцию.
//При реализации нужно рекурсивно разделить контейнер на несколько блоков и для каждого запустить отдельную задачу, применяющую обрабатывающую функцию к части контейнера.


#include <iostream>
#include <future>
#include <thread>


int incrementVec(int value) {
	return value + 1;
}

template<typename Iterator, typename Function>
void asyncForEach(Iterator begin, Iterator end, Function func) {
	auto length = std::distance(begin, end);
	if (!length)
		return;
	auto minPerThread = 10;
	if (length < (2 * minPerThread)) {
		std::for_each(begin, end, func);
	}
	else {
		Iterator middle = begin + length / 2;
		std::future<void> firstHalf = std::async(&asyncForEach<Iterator, Function>, begin, middle, func);
		std::future<void> lastHalf = std::async(&asyncForEach<Iterator, Function>, middle, end, func);
		firstHalf.get();
		lastHalf.get();
	}
}
void printVec(std::vector<int>& vec) {
	for (const auto& elem : vec) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}
int main()
{
	std::vector<int> vec{ 1,2,3,4,5,6,7,8,9,10 };
	printVec(vec);
	asyncForEach(vec.begin(), vec.end(), [](int value) {std::cout << incrementVec(value) << " "; });
}
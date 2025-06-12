//Задание 1
//Сортировка выбором
//Реализуйте алгоритм сортировки выбором.
//Сортировка выбором состоит в том, что в неупорядоченном списке находится наименьший элемент. Выбранный в исходном списке минимальный элемент записывается на i-е место исходного списка (i=1,2,…,п), а элемент с i-го места — на место выбранного. При этом очевидно, что уже упорядоченные элементы, а они будут расположены, начиная с первого места, исключаются из дальнейшей сортировки, поэтому длина списка, участвующего в каждом последующем просмотре, должна быть на один элемент меньше предыдущего.
//Поиск минимального элемента реализуйте в виде асинхронной задачи.
//Результат должен возвращаться в основной поток через связку std::promise-std::future.

#include <vector>
#include <future>
#include <algorithm>
#include <iostream>

using namespace std;

void print(const vector<int>& vec) {
	for (const auto& i : vec) {
		cout << i << " ";
	}
	cout << endl;
}

void findMin(const vector<int> vec, int i, const int vSize, promise<int> prom) {
	int minIndex = i;

	for (int j = i + 1; j < vSize; j++) {
		if (vec[j] < vec[minIndex]) {
			minIndex = j;
		}
	}
	prom.set_value(minIndex);
};


int main() {
	vector<int> vec{ 11, 6, 2, 3, 5, 3, 9, 4, 1, 8, 7, 10, 12 };

	int vSize = vec.size();

	cout << "Before sort: ";
	print(vec);

	for (int i = 0; i < vSize; i++) {
		promise<int> prom;
		future<int> future_res = prom.get_future();

		auto asyncFindMin = async(launch::async, findMin, vec, i, vSize, move(prom));

		int minIndex = future_res.get();
		if (vec[i] != vec[minIndex]) {
			swap(vec[i], vec[minIndex]);
		}
	}
	cout << "After sort: ";
	print(vec);
}
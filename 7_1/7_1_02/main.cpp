//#### Параллельные вычисления
//
//- Напишите программу для расчёта суммы двух векторов.
//- Распараллельте эту программу на 2, 4, 8 и 16 потоков.
//- Определите, какое количество потоков даёт самый быстрый результат.
//- Сравните результаты выполнения для массивов из 1 000, 10 000, 100 000 и 1 000 000 элементов.
//- Время выполнения для каждого варианта сведите в таблицу и выведите её в консоль.
//- Первый запущенный поток должен вывести на экран доступное количество аппаратных ядер.

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <Windows.h>

using namespace std;

// Массивы делим по количеству потоков и считаем сумму только по части массива
void sum(vector<int>& v1, vector<int>& v2, int n, int cnt) {
	int res = 0;

	for (int i = (v1.size() / n) * cnt - (v1.size() / n); i < (v1.size() / n) * cnt; i++) {
		res = res + v1[i] + v2[i];
	}
}

// Заполнение контейнера случайными числами
void filling_vector(vector<int>& v, int num_elem) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, num_elem);
	auto rand_num([=]() mutable {return dis(gen); });
	generate(v.begin(), v.end(), rand_num);
}

int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	cout << "Количество аппаратных ядер - " << thread::hardware_concurrency() << endl;

	int arr_num_thread[] = { 1,2,4,8,16 };
	int arr_num_elem[] = { 1000,10000,100000,1000000 };

	cout << " \t \t";
	for (int num_elem : arr_num_elem) {
		cout << num_elem << " \t \t";
	}
	cout << endl;

	for (int num_thread : arr_num_thread) {
		cout << num_thread << " потоков\t";
		for (int num_elem : arr_num_elem) {
			
			// Создание массивов для данных
			vector<int> v1(num_elem); 
			vector<int> v2(num_elem);

			// Заполнение массивов случаййными числами
			filling_vector(v1, num_elem);
			filling_vector(v2, num_elem);

			// Старт отсчета времени
			auto start = std::chrono::steady_clock::now();

			// Создание потоков для обработки массивов
			vector<thread> vt;
			for (int i = 1; i <= num_thread; i++) {
				vt.push_back(thread(sum, ref(v1), ref(v2), num_thread, i));
			}
			for (auto& t: vt) {
				t.join();
			}
			
			// Остановка отсчета времени
			auto end = std::chrono::steady_clock::now();
			chrono::duration<double,milli> elapsed_seconds = end - start;
			
			std::cout << elapsed_seconds.count() << "ms\t";
		}
		cout << endl;
	}
	return 0;
}
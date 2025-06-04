//Задание 2
//Прогресс - бар
//Создайте консольное приложение для имитации многопоточного расчёта.
//Количество потоков, длина расчёта должны быть заданы переменными.
//В консоль во время работы программы должны построчно для каждого потока выводиться :
//
//• номер потока по порядку;
//
//• идентификатор потока;
//
//• заполняющийся индикатор наподобие прогресс - бара, визуализирующий процесс «расчёта»;
//
//• после завершения работы каждого потока в соответствующей строке суммарное время, затраченное на работу потока.
//Строки прогресс - баров каждого потока должны выводиться одновремено.Время появления каждого нового символа 
//в строке прогресс - бара подберите так, чтобы процесс заполнения строки был виден.Пример работы программы по ссылке.
//
//Дополнение к заданию 2 *
//Во время очередной итерации «расчёта» сымитируйте со случайной вероятностью возникновение ошибки(exception), 
//которая не должна приводить к прекращению работы потока или программы.При этом этот факт должен визуализироваться 
//отдельным цветом на прогресс - баре.

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "myconsole.h"

using namespace std;

mutex mtx;

void progress(int n, int th, int cnt) {
	mtx.lock();
	cout << n << "\t" << this_thread::get_id() << endl;
	mtx.unlock();
	
	// Старнт отсчета времени
	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < cnt; i++) {
		this_thread::sleep_for(300ms);
		mtx.lock();
		consol_parameter::SetPosition(16 + i, n + 1);
		cout << u8"▒";
		mtx.unlock();
	}
	
	// Остановка отсчета времени
	auto end = std::chrono::steady_clock::now();
	chrono::duration<double, milli> elapsed_seconds = end - start;
	
	mtx.lock();
	
	// Вывод времени в консоль
	consol_parameter::SetPosition(32, n + 1);
	std::cout << elapsed_seconds.count() << "ms" << endl;

	// Перевод позиции в самый низ
	consol_parameter::SetPosition(0, th+1);
	
	mtx.unlock();
}

int main() {

	SetConsoleOutputCP(CP_UTF8);

	int num_thread = 3; // Количество потоков
	int cnt = 12;

	cout << "#\tId\tProgress Bar\tTime" << endl;
	this_thread::sleep_for(10ms);

	thread* thread_arr = new thread[num_thread];
	for (int i = 0; i < num_thread; i++) {
		thread_arr[i] = thread(progress, i, num_thread, cnt);
	}

	for (int j = 0; j < num_thread; j++) {
		thread_arr[j].join();
	}

	return 0;
}
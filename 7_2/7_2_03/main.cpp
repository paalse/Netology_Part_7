//Задание 3
//Защищённый обмен данными
//Создайте класс Data, содержащий в качестве полей скалярные данные и мьютекс.
//Создайте функцию swap, которая принимает ссылки на два объекта класса Data и обменивает их местами.
//В функциях нужно сначала захватить мьютексы обоих объектов, а затем выполнить обмен данными.
//Реализуйте три варианта этой функции : при помощи lock, scoped_lock и unique_lock.
#include <iostream>
#include <Thread>
#include "Data.h"


void swapLock(Data& A, Data& B)
{
    std::lock(A.mtx, B.mtx);
        
    int tmp = A.get_data();
    A.set_data(B.get_data());
    B.set_data(tmp);

    std::cout << "After swap with lock:\n A:" << A.get_data() << " B:" << B.get_data() << std::endl;
    
    A.mtx.unlock();
    B.mtx.unlock();
}

void swapScopedLock(Data& A, Data& B)
{
    std::scoped_lock lock(A.mtx, B.mtx);

    int tmp = A.get_data();
    A.set_data(B.get_data());
    B.set_data(tmp);

    std::cout << "After swap with scoped_lock:\n A:" << A.get_data() << " B:" << B.get_data() << std::endl;
}

void swapUniqueLock(Data& A, Data& B)
{
    std::unique_lock<std::mutex> lock1(A.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock2(B.mtx, std::defer_lock);
    lock1.lock();
    lock2.lock();

    int tmp = A.get_data();
    A.set_data(B.get_data());
    B.set_data(tmp);

    std::cout << "After swap with unique_lock:\n A:" << A.get_data() << " B:" << B.get_data() << std::endl;

    lock1.unlock();
    lock2.unlock();
}


int main()
{
	Data A(10);
	Data B(15);
	std::cout << "Before swap:\n A:" << A.get_data() << " B:" << B.get_data() << std::endl;

    std::thread t1(swapLock, std::ref(A), std::ref(B));
    std::thread t2(swapUniqueLock, std::ref(A), std::ref(B));
    std::thread t3(swapUniqueLock, std::ref(A), std::ref(B));
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
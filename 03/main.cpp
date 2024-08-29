#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <utility>

class Data {
public:
    int value;
    std::mutex mtx;

    Data(int val) : value(val) {}
};


void swap_lock(Data& a, Data& b) {
    std::lock(a.mtx, b.mtx);
    std::lock_guard<std::mutex> lock_a(a.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(b.mtx, std::adopt_lock);
    std::swap(a.value, b.value);
}


void swap_scoped_lock(Data& a, Data& b) {
    std::scoped_lock lock(a.mtx, b.mtx);
    std::swap(a.value, b.value);
}


void swap_unique_lock(Data& a, Data& b) {
    std::unique_lock<std::mutex> lock_a(a.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock_b(b.mtx, std::defer_lock);
    std::lock(lock_a, lock_b);
    std::swap(a.value, b.value);
}


void thread_function(Data& a, Data& b, void (*swap_func)(Data&, Data&)) {
    swap_func(a, b);
}

int main() {

    Data data1(10);
    Data data2(20);

    std::cout << "Initial values:\n";
    std::cout << "data1: " << data1.value << "\n";
    std::cout << "data2: " << data2.value << "\n";


    const int num_threads = 9;
    std::vector<std::thread> threads;


    std::cout << "\nUsing swap_lock:\n";
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, std::ref(data1), std::ref(data2), swap_lock);
    }


    for (auto& t : threads) {
        t.join();
    }

    std::cout << "data1: " << data1.value << "\n";
    std::cout << "data2: " << data2.value << "\n";

    data1.value = 10;
    data2.value = 20;

    std::cout << "\nUsing swap_scoped_lock:\n";
    threads.clear();
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, std::ref(data1), std::ref(data2), swap_scoped_lock);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "data1: " << data1.value << "\n";
    std::cout << "data2: " << data2.value << "\n";


    data1.value = 10;
    data2.value = 20;


    std::cout << "\nUsing swap_unique_lock:\n";
    threads.clear();
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, std::ref(data1), std::ref(data2), swap_unique_lock);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "data1: " << data1.value << "\n";
    std::cout << "data2: " << data2.value << "\n";

    return 0;
}

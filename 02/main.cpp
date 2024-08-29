
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>


std::condition_variable cv{};
const int progress_length = 4;

std::mutex console_mutex;

void threadFunction(int threadNumber, int numOfThreads) {
std::lock_guard<std::mutex> guard(console_mutex);
     auto start_time = std::chrono::steady_clock::now();

     std::cout << threadNumber<< "  " << std::this_thread::get_id()<<"   ";

    for (int i = 0; i < progress_length; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate work

        // Locking the console for thread-safe output
        // std::lock_guard<std::mutex> guard(console_mutex);


        for (int j = 0; j <= i; ++j) std::cout << "[]";
    }

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    {
    //    std::lock_guard<std::mutex> guard(console_mutex);
        std::cout<<"   " << elapsed_seconds.count() << " seconds. \n";
    }

}


void runFunction(){
    int N =5;
    std::thread* t = new std::thread[N];
    for(size_t i = 0; i < N; i++){
        t[i] = std::thread(threadFunction,i,N);
    }
    for(size_t i = 0; i < N; i++)
        t[i].join();
}

int main() {
    std::cout << "#    id       Progress Bar            Time" << std::endl;

   runFunction();


    return 0;
}


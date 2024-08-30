
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <random>
#include <sstream>
#include <iomanip>


std::mutex console_mutex;

int lower_delay_var = 200;
int higher_delay_var = 500;

void move_cursor_to(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void threadFunction(int threadNumber, int length , int y_cursor_pos) {
    std::random_device random;
    std::mt19937 generate(random());
    std::uniform_int_distribution<> delay(lower_delay_var, higher_delay_var);

     auto start_time = std::chrono::steady_clock::now();
    std::ostringstream progress_bar;
     progress_bar << threadNumber<< "  " << std::this_thread::get_id()<<"   ";

    for (int i = 0; i < length; ++i) {
         std::this_thread::sleep_for(std::chrono::milliseconds(delay(generate)));  // Simulate work


         std::lock_guard<std::mutex> guard(console_mutex);
        move_cursor_to(0, y_cursor_pos);
        progress_bar << "[]";
        std::cout << progress_bar.str() << std::setw(length - i) << std::flush;
    }

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    {
        std::lock_guard<std::mutex> guard(console_mutex);
        move_cursor_to(0, y_cursor_pos);
        //std::cout<<"   " << elapsed_seconds.count() << " seconds. \n";
        std::cout << progress_bar.str() << std::setw(length - 10) << "-" << elapsed_seconds.count() << "seconds\n";
    }

}




int main() {

    int length  = 20;
    int numOfThreads = 5;
    // std::thread* t = new std::thread[numOfThreads];
    std::vector<std::thread> threads;
    for(size_t i = 0; i < numOfThreads; i++){
        threads.emplace_back(threadFunction,i,length,i);
    }
    for(auto& thread : threads)
        thread.join();

    std::cout << "#    id                    Progress Bar                  Time" << "\n";
    std::cin.get();
    return 0;
}


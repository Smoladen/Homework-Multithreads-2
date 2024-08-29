#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

int maxClients = 10;

void clientImitator(std::atomic<int>& numOfClients) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        int currentClients = numOfClients.load(std::memory_order_relaxed);

        if (currentClients < maxClients) {
            numOfClients.fetch_add(1, std::memory_order_relaxed);
            std::cout << "Current number of Clients: " << numOfClients.load(std::memory_order_relaxed) << std::endl;
        } else {
            std::cout << "Reached limit. Current Number of Clients: " << numOfClients.load(std::memory_order_relaxed) << std::endl;
            break;
        }
    }
}


void operatorImitator(std::atomic<int>& numOfClients) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        int currentClients = numOfClients.load(std::memory_order_relaxed);

        if (currentClients > 0) {
            numOfClients.fetch_sub(1, std::memory_order_relaxed);
            std::cout << "Client served. Clients Left: " << numOfClients.load(std::memory_order_relaxed) << std::endl;
        } else {
            std::cout << "No more clients to serve. Exiting." << std::endl;
            break;
        }
    }
}

int main() {
    std::atomic<int> numOfClients(0);

    std::thread clientThread(clientImitator, std::ref(numOfClients));
    std::thread operatorThread(operatorImitator, std::ref(numOfClients));

    clientThread.join();
    operatorThread.join();

    return 0;
}

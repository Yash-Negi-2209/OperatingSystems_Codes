
//Reg No. 21BCB0231

#include <iostream>
#include <pthread.h>
#include <mutex>
#include <condition_variable>

using namespace std;

const int C = 3; // Maximum capacity of the car
const int n = 10; // Total number of passengers

mutex mtx, mtx2;
int boarders = 0, unboarders = 0;
condition_variable boardQueue, unboardQueue, allAboard, allAshore;

void passengerThread(int id) {
    // Wait for the car to load
    unique_lock<mutex> lock(mtx);
    boardQueue.wait(lock);

    // Board the car
    boarders++;
    cout << "Passenger " << id << " boarded the car." << endl;

    // If all passengers have boarded, signal the car to depart
    if (boarders == C) {
        allAboard.notify_one();
    }

    lock.unlock();

    // Wait for the car to unload
    unique_lock<mutex> lock2(mtx2);
    unboardQueue.wait(lock2);

    // Unboard the car
    unboarders++;
    cout << "Passenger " << id << " unboarded the car." << endl;

    // If all passengers have unboarded, signal the car to continue
    if (unboarders == C) {
        allAshore.notify_one();
    }
}



void carThread() {
    while (true) {
        // Wait for passengers to board
        unique_lock<mutex> lock(mtx);
        if (boarders < C) {
            boardQueue.wait(lock);
        }

        // Load the car and signal passengers to unboard
        cout << "Car loaded. All aboard!" << endl;
        lock.unlock();

        // Signal passengers to unboard
        mtx2.lock();
        unboardQueue.notify_all();
        mtx2.unlock();

        // Wait for all passengers to unboard
        unique_lock<mutex> lock2(mtx2);
        if (unboarders < C) {
            allAshore.wait(lock2);
        }

        // Unload the car and reset counters
        cout << "Car unloaded. All ashore!" << endl;
        boarders = 0;
        unboarders = 0;

        // Signal passengers to board again
        mtx.lock();
        boardQueue.notify_all();
        mtx.unlock();
    }
}



int main() {
    thread car(carThread);
    thread passengers[n];

    // Start passenger threads
    for (int i = 0; i < n; i++) {
        passengers[i] = thread(passengerThread, i);
    }

    // Wait for all passenger threads to finish
    for (int i = 0; i < n; i++) {
        passengers[i].join();
    }

    // Stop the car thread
    car.join();

    return 0;
}

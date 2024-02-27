#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

const int C = 3;  // Maximum number of passengers in the car
const int T = 5;  // Number of rides

mutex mtx;  // Mutex for synchronization
condition_variable car_full;  // Condition variable for the car to start the ride
condition_variable passengers_unboarded;  // Condition variable for passengers to unboard
condition_variable passengers_boarded;  // Condition variable for passengers to board
int passenger_count = 0;  // Count of passengers in the car

void passengerThread(int id) {
    // Wander around the amusement park
    this_thread::sleep_for(chrono::seconds(1));

    unique_lock<mutex> lock(mtx);
    
    // Wait until it's their turn to board
    passengers_boarded.wait(lock, [id]() { return (passenger_count < C); });

    passenger_count++;
    cout << "Passenger " << id << " boarded. Passenger count: " << passenger_count << endl;

    if (passenger_count == C) {
        // Car is full, signal the car to start the ride
        car_full.notify_one();
    }

    // Wait for the ride to complete
    passengers_unboarded.wait(lock, [id]() { return (passenger_count == 0); });

    passenger_count--;
    cout << "Passenger " << id << " unboarded. Passenger count: " << passenger_count << endl;

    // Notify the next batch of passengers to board
    if (passenger_count == 0) {
        passengers_boarded.notify_all();
    }
}

void carThread() {
    for (int ride = 0; ride < T; ride++) {
        unique_lock<mutex> lock(mtx);
        
        // Wait until the car is full
        car_full.wait(lock, []() { return (passenger_count == C); });

        cout << "Car ride in progress..." << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Car ride complete!" << endl;

        // Notify passengers to unboard
        passengers_unboarded.notify_all();

        // Wait until all passengers have unboarded
        passengers_boarded.wait(lock, []() { return (passenger_count == 0); });

        // Notify the next batch of passengers to board
        passengers_boarded.notify_all();
    }
}

int main() {
    thread car(carThread);

    thread passengers[10];  // Assuming there are 10 passengers
    for (int i = 0; i < 10; i++) {
        passengers[i] = thread(passengerThread, i);
    }

    for (int i = 0; i < 10; i++) {
        passengers[i].join();
    }

    car.join();

    return 0;
}

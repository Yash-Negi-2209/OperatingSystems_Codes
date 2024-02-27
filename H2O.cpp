#include <iostream>
#include <thread>
#include <semaphore.h>
sem_t hydrogen_sem;
sem_t oxygen_sem;
int h_count = 0;
int o_count = 0;
void hydrogen() {
sem_wait(&hydrogen_sem); // Wait for a hydrogen permit
h_count++; // Increment hydrogen atom count
// Bond with an oxygen thread to form a water molecule
std::cout << "Hydrogen atom bonded." << std::endl;
sem_post(&oxygen_sem); // Signal oxygen_sem to release permit
}
void oxygen() {
sem_wait(&oxygen_sem); // Wait for an oxygen permit
o_count++; // Increment oxygen atom count
// Bond with two hydrogen threads to form a water molecule
std::cout << "Oxygen atom bonded." << std::endl;
if (h_count >= 2 && o_count >= 1) {
h_count -= 2;
o_count--;
std::cout << "Water (H2O) molecule formed!" << std::endl;
// Release permits for the next set of threads
sem_post(&hydrogen_sem);
sem_post(&hydrogen_sem);
sem_post(&oxygen_sem);
} else {
sem_post(&oxygen_sem); // Release permit if not enough atoms
}
}
int main() {
sem_init(&hydrogen_sem, 0, 2); // Initialize hydrogen_sem with 2 permits
sem_init(&oxygen_sem, 0, 1); // Initialize oxygen_sem with 1 permit
std::thread hydrogen_threads[10];
std::thread oxygen_threads[5];
// Create and start hydrogen threads
for (int i = 0; i < 10; ++i) {
hydrogen_threads[i] = std::thread(hydrogen);
}
// Create and start oxygen threads
for (int i = 0; i < 5; ++i) {
oxygen_threads[i] = std::thread(oxygen);
}
// Wait for all threads to finish
for (int i = 0; i < 10; ++i) {
hydrogen_threads[i].join();
}
for (int i = 0; i < 5; ++i) {
oxygen_threads[i].join();
}
sem_destroy(&hydrogen_sem);
sem_destroy(&oxygen_sem);
return 0;
}
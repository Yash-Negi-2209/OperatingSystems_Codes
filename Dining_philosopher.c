#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PHILOSOPHERS 10 // Maximum number of philosophers

enum { THINKING, HUNGRY, EATING } state[MAX_PHILOSOPHERS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition[MAX_PHILOSOPHERS];

int numPhilosophers;

void test(int philosopherID) {
    if (state[philosopherID] == HUNGRY &&
        state[(philosopherID + numPhilosophers - 1) % numPhilosophers] != EATING &&
        state[(philosopherID + 1) % numPhilosophers] != EATING) {

        state[philosopherID] = EATING;
        printf("Philosopher %d is eating.\n", philosopherID);
        pthread_cond_signal(&condition[philosopherID]);
    }
}

void pickup_forks(int philosopherID) {
    pthread_mutex_lock(&mutex);
    state[philosopherID] = HUNGRY;
    printf("Philosopher %d is hungry.\n", philosopherID);
    test(philosopherID);

    while (state[philosopherID] != EATING) {
        pthread_cond_wait(&condition[philosopherID], &mutex);
    }

    pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopherID) {
    pthread_mutex_lock(&mutex);
    state[philosopherID] = THINKING;
    printf("Philosopher %d is thinking.\n", philosopherID);
    test((philosopherID + numPhilosophers - 1) % numPhilosophers);
    test((philosopherID + 1) % numPhilosophers);
    pthread_mutex_unlock(&mutex);
}

void* philosopher(void* arg) {
    int philosopherID = *((int*)arg);

    for (int i = 0; i < 3; i++) {
        sleep(1); // Thinking

        pickup_forks(philosopherID);

        sleep(1); // Eating

        return_forks(philosopherID);
    }

    pthread_exit(NULL);
}

int main() {
    printf("Enter the number of philosophers (1-%d): ", MAX_PHILOSOPHERS);
    scanf("%d", &numPhilosophers);

    if (numPhilosophers < 1 || numPhilosophers > MAX_PHILOSOPHERS) {
        printf("Invalid number of philosophers. Exiting...\n");
        return 1;
    }

    pthread_t philosopherThreads[MAX_PHILOSOPHERS];
    int philosopherIDs[MAX_PHILOSOPHERS];

    // Initialize conditions
    for (int i = 0; i < numPhilosophers; i++) {
        pthread_cond_init(&condition[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < numPhilosophers; i++) {
        philosopherIDs[i] = i;
        pthread_create(&philosopherThreads[i], NULL, philosopher, (void*)&philosopherIDs[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < numPhilosophers; i++) {
        pthread_join(philosopherThreads[i], NULL);
    }

    // Destroy conditions
    for (int i = 0; i < numPhilosophers; i++) {
        pthread_cond_destroy(&condition[i]);
    }

    return 0;
}

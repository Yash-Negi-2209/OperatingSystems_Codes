#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define N 2 // Number of processes

int turn = 0;
bool flag[N] = {false}; // Array to indicate if a process wants to enter the critical section

void enterCriticalSection(int processID) {
    int other = 1 - processID; // Index of the other process

    flag[processID] = true; // Process i wants to enter the critical section
    turn = processID; // Set the turn to process i

    // Wait until it's process i's turn and the other process doesn't want to enter
    while (flag[other] && turn == processID)
    {
        // Busy wait
    }

    // Critical section
    printf("Process %d entered the critical section.\n", processID);

    // Exit section
    flag[processID] = false;
}

void* process(void* arg) {
    int processID = *((int*)arg);
    // Perform non-critical section work

    // Enter critical section
    enterCriticalSection(processID);

    // Perform non-critical section work

    // Exit the process
    printf("Process %d exited.\n", processID);
}

int main() {
    pthread_t threads[N];
    int threadIDs[N] = {0, 1};

    // Create two threads representing two processes
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, process, (void*)&threadIDs[i]);
    }

    // Join the threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

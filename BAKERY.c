#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

#define MAX_THREADS 10

atomic_int choosing[MAX_THREADS] = {0}; // Array to indicate if a process is choosing a ticket number
atomic_int ticket[MAX_THREADS] = {0};   // Array to hold the ticket number of each process

int numThreads;
int criticalSectionWork = 5; // Number of seconds the critical section work takes

void enterCriticalSection(int processID) {
    choosing[processID] = 1; // Process i is choosing a ticket number
    atomic_thread_fence(memory_order_seq_cst); // Memory fence to ensure ordering

    // Find the maximum ticket number and assign a higher ticket number to the current process
    int maxTicket = 0;
    for (int i = 0; i < numThreads; i++) {
        int currentTicket = atomic_load(&ticket[i]);
        if (currentTicket > maxTicket) {
            maxTicket = currentTicket;
        }
    }
    ticket[processID] = maxTicket + 1;

    atomic_thread_fence(memory_order_seq_cst); // Memory fence to ensure ordering
    choosing[processID] = 0;

    // Check if any other process is choosing a ticket number and wait until it's finished
    for (int j = 0; j < numThreads; j++) {
        while (choosing[j] == 1) {
            // Busy wait
        }
        atomic_thread_fence(memory_order_seq_cst); // Memory fence to ensure ordering

        // Wait until it's the current process's turn based on the ticket number
        while ((ticket[j] != 0) && ((ticket[j] < ticket[processID]) || ((ticket[j] == ticket[processID]) && j < processID))) {
            // Busy wait
        }
    }

    // Critical section
    printf("Process %d entered the critical section.\n", processID);
    sleep(criticalSectionWork); // Simulating the critical section work

    // Reset the ticket number to 0 to indicate the process has exited the critical section
    ticket[processID] = 0;
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
    pthread_t threads[MAX_THREADS];
    int threadIDs[MAX_THREADS];

    // Get the number of threads (processes) from the user
    printf("Enter the number of threads (max 10): ");
    scanf("%d", &numThreads);

    // Validate the number of threads
    if (numThreads < 1 || numThreads > MAX_THREADS) {
        printf("Invalid number of threads. Exiting.\n");
        return 1;
    }

    // Get the critical section work duration from the user
    printf("Enter the duration of the critical section work (in seconds): ");
    scanf("%d", &criticalSectionWork);

    // Create threads representing the processes
    for (int i = 0; i < numThreads; i++) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, process, (void*)&threadIDs[i]);
    }

    // Join the threads
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}


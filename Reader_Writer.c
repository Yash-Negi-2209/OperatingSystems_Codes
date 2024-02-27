#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_READERS 10  // Maximum number of reader threads
#define MAX_WRITERS 10  // Maximum number of writer threads
#define MAX_COUNT 10    // Maximum number of times each thread reads/writes

int sharedData = 0;       // Shared data
int activeReaders = 0;    // Number of active reader threads
int waitingWriters = 0;   // Number of waiting writer threads
int activeWriters = 0;    // Number of active writer threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;     // Mutex for reader-writer synchronization
pthread_cond_t canRead = PTHREAD_COND_INITIALIZER;     // Condition variable for readers
pthread_cond_t canWrite = PTHREAD_COND_INITIALIZER;    // Condition variable for writers

void* reader(void* arg) {
    int readerID = *((int*)arg);
    for (int i = 0; i < MAX_COUNT; i++) {
        // Entry section
        pthread_mutex_lock(&mutex);
        while (activeWriters > 0 || waitingWriters > 0) {
            // Wait until there are no active writers or waiting writers
            pthread_cond_wait(&canRead, &mutex);
        }
        activeReaders++;

        // Critical section
        printf("Reader %d read data: %d (%d/%d)\n", readerID, sharedData, i + 1, MAX_COUNT);

        // Exit section
        activeReaders--;
        if (activeReaders == 0) {
            // Signal waiting writers if there are no active readers
            pthread_cond_signal(&canWrite);
        }
        pthread_mutex_unlock(&mutex);

        // Remainder section
        // Perform other non-critical work
    }
    pthread_exit(NULL);
}

void* writer(void* arg) {
    int writerID = *((int*)arg);
    for (int i = 0; i < MAX_COUNT; i++) {
        // Entry section
        pthread_mutex_lock(&mutex);
        waitingWriters++;

        while (activeReaders > 0 || activeWriters > 0) {
            // Wait until there are no active readers or writers
            pthread_cond_wait(&canWrite, &mutex);
        }
        waitingWriters--;
        activeWriters++;

        // Critical section
        sharedData++;
        printf("Writer %d wrote data: %d (%d/%d)\n", writerID, sharedData, i + 1, MAX_COUNT);

        // Exit section
        activeWriters--;
        if (waitingWriters > 0) {
            // Signal waiting writers
            pthread_cond_signal(&canWrite);
        } else {
            // Signal waiting readers if there are no waiting writers
            pthread_cond_broadcast(&canRead);
        }
        pthread_mutex_unlock(&mutex);

        // Remainder section
        // Perform other non-critical work
    }
    pthread_exit(NULL);
}

int main() {
    int numReaders, numWriters;
    printf("Enter the number of reader threads (1-%d): ", MAX_READERS);
    scanf("%d", &numReaders);

    printf("Enter the number of writer threads (1-%d): ", MAX_WRITERS);
    scanf("%d", &numWriters);

    if (numReaders < 1 || numReaders > MAX_READERS || numWriters < 1 || numWriters > MAX_WRITERS) {
        printf("Invalid number of threads. Exiting...\n");
        return 1;
    }

    pthread_t readerThreads[MAX_READERS];
    pthread_t writerThreads[MAX_WRITERS];
    int readerIDs[MAX_READERS];
    int writerIDs[MAX_WRITERS];

    // Create reader threads
    for (int i = 0; i < numReaders; i++) {
        readerIDs[i] = i + 1;
        pthread_create(&readerThreads[i], NULL, reader, (void*)&readerIDs[i]);
    }

    // Create writer threads
    for (int i = 0; i < numWriters; i++) {
        writerIDs[i] = i + 1;
        pthread_create(&writerThreads[i], NULL, writer, (void*)&writerIDs[i]);
    }

    // Join reader threads
    for (int i = 0; i < numReaders; i++) {
        pthread_join(readerThreads[i], NULL);
    }

    // Join writer threads
    for (int i = 0; i < numWriters; i++) {
        pthread_join(writerThreads[i], NULL);
    }

    return 0;
}

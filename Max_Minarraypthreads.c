#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

// Structure to pass data to thread function
typedef struct {
    int* array;
    int size;
    int min;
    int max;
} ThreadData;

// Thread function to find the minimum and maximum numbers within a section
void* FindMinMax(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    // Initialize min and max with the first element in the section
    data->min = data->array[0];
    data->max = data->array[0];

    // Find min and max in the section
    for (int i = 1; i < data->size; i++) {
        if (data->array[i] < data->min) {
            data->min = data->array[i];
        }
        if (data->array[i] > data->max) {
            data->max = data->array[i];
        }
    }

    pthread_exit(NULL);
}

int main() {
    int size;
    printf("Enter the size of the array: ");
    scanf("%d", &size);

    int* array = (int*)malloc(size * sizeof(int));

    printf("Enter the elements of the array:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &array[i]);
    }

    pthread_t threads[NUM_THREADS]; // Thread IDs
    ThreadData threadData[NUM_THREADS]; // Data for each thread

    // Divide the array into sections for each thread
    int sectionSize = size / NUM_THREADS;
    int remainder = size % NUM_THREADS;
    int start = 0;

    // Create threads and assign sections
    for (int i = 0; i < NUM_THREADS; i++) {
        int end = start + sectionSize - 1;

        // Adjust the end index of the last thread if there is a remainder
        if (i == NUM_THREADS - 1) {
            end += remainder;
        }

        threadData[i].array = &array[start];
        threadData[i].size = end - start + 1;

        pthread_create(&threads[i], NULL, FindMinMax, (void*)&threadData[i]);

        start = end + 1;
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Find the overall minimum and maximum numbers
    int overallMin = threadData[0].min;
    int overallMax = threadData[0].max;

    for (int i = 1; i < NUM_THREADS; i++) {
        if (threadData[i].min < overallMin) {
            overallMin = threadData[i].min;
        }
        if (threadData[i].max > overallMax) {
            overallMax = threadData[i].max;
        }
    }

    // Print the results
    printf("Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    printf("Minimum number: %d\n", overallMin);
    printf("Maximum number: %d\n", overallMax);

    free(array);

    return 0;
}

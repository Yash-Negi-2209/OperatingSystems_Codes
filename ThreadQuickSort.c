//Program-2: Write a program to sort an array using Pthreads.
// Divide the array into multiple sub-arrays and assign each sub-array 
// to a different thread to sort it using a sorting algorithm 
// such as bubble sort or quicksort.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 8

typedef struct {
    int* arr;
    int start;
    int end;
} SortParams;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

void* sort_thread(void* params) {
    SortParams* sp = (SortParams*)params;
    quicksort(sp->arr, sp->start, sp->end);
    return NULL;
}

void merge(int* arr, int start, int mid, int end) {
    int i, j, k;
    int n1 = mid - start + 1;
    int n2 = end - mid;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[start + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = start;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void merge_sort(int* arr, int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;

        merge_sort(arr, start, mid);
        merge_sort(arr, mid + 1, end);

        merge(arr, start, mid, end);
    }
}

void sort_array(int* arr, int size) {
    if (size <= 1)
        return;

    pthread_t threads[MAX_THREADS];
    SortParams params[MAX_THREADS];
    int thread_count = size < MAX_THREADS ? size : MAX_THREADS;
    int chunk_size = size / thread_count;
    int remaining = size % thread_count;
    int start = 0;

    for (int i = 0; i < thread_count; i++) {
        int current_chunk_size = chunk_size + (i < remaining ? 1 : 0);
        params[i].arr = arr;
        params[i].start = start;
        params[i].end = start + current_chunk_size - 1;
        start += current_chunk_size;
        pthread_create(&threads[i], NULL, sort_thread, &params[i]);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    merge_sort(arr, 0, size - 1);
}

int main() {
    int arr[] = { 99, 72, 47, 14, 85, 91, 38, 23, 6 };
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    sort_array(arr, size);

    printf("Sorted array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

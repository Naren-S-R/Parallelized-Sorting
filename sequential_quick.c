#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

int random_partition(int arr[], int low, int high) {
    int pivot_idx = low + rand() % (high - low + 1);
    swap(&arr[pivot_idx], &arr[high]);
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++)
        if (arr[j] <= pivot) swap(&arr[++i], &arr[j]);
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = random_partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s <size> <input_file> [--test]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int test_mode = argc >= 4 && strcmp(argv[3], "--test") == 0;

    int *array = malloc(n * sizeof(int));
    FILE *f = fopen(argv[2], "r");
    for (int i = 0; i < n; i++) fscanf(f, "%d", &array[i]);
    fclose(f);

    srand(time(NULL));
    clock_t start = clock();
    quickSort(array, 0, n - 1);
    clock_t end = clock();

    fprintf(stderr, "Sequential Quick Sort Time: %f sec\n", (double)(end - start) / CLOCKS_PER_SEC);
    if (test_mode)
        for (int i = 0; i < n; i++) printf("%d\n", array[i]);

    free(array);
    return 0;
}

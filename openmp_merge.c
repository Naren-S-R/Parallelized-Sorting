#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = malloc(n1 * sizeof(int)), *R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void parallelMergeSort(int arr[], int l, int r, int depth) {
    if (l < r) {
        int m = (l + r) / 2;
        if (depth > 0) {
            #pragma omp task
            parallelMergeSort(arr, l, m, depth - 1);
            #pragma omp task
            parallelMergeSort(arr, m + 1, r, depth - 1);
            #pragma omp taskwait
        } else {
            parallelMergeSort(arr, l, m, 0);
            parallelMergeSort(arr, m + 1, r, 0);
        }
        merge(arr, l, m, r);
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
    double start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(array, 0, n - 1, 4);
    }
    double end = omp_get_wtime();

    fprintf(stderr, "OpenMP Merge Sort Time: %f sec\n", end - start);
    if (test_mode)
        for (int i = 0; i < n; i++) printf("%d\n", array[i]);

    free(array);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
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

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
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

    clock_t start = clock();
    mergeSort(array, 0, n - 1);
    clock_t end = clock();

    fprintf(stderr, "Sequential Merge Sort Time: %f sec\n", (double)(end - start) / CLOCKS_PER_SEC);
    if (test_mode)
        for (int i = 0; i < n; i++) printf("%d\n", array[i]);

    free(array);
    return 0;
}

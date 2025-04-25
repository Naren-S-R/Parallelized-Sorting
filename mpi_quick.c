#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void merge(int *a, int n1, int *b, int n2, int *res) {
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) res[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    while (i < n1) res[k++] = a[i++];
    while (j < n2) res[k++] = b[j++];
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 3) {
        if (rank == 0) printf("Usage: %s <size> <input_file> [--test]\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    int test_mode = argc >= 4 && strcmp(argv[3], "--test") == 0;
    const char *input_file = argv[2];

    int local_n = n / size;
    int *full = NULL, *local = malloc(local_n * sizeof(int));

    if (rank == 0) {
        full = malloc(n * sizeof(int));
        FILE *f = fopen(input_file, "r");
        for (int i = 0; i < n; i++) fscanf(f, "%d", &full[i]);
        fclose(f);
    }

    MPI_Scatter(full, local_n, MPI_INT, local, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    qsort(local, local_n, sizeof(int), compare); 
    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();
    qsort(local, local_n, sizeof(int), compare);

    int step;
    for (step = 1; step < size; step *= 2) {
        if (rank % (2 * step) == 0) {
            int partner = rank + step;
            if (partner < size) {
                int partner_size;
                MPI_Recv(&partner_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int *partner_data = malloc(partner_size * sizeof(int));
                MPI_Recv(partner_data, partner_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int *merged = malloc((local_n + partner_size) * sizeof(int));
                merge(local, local_n, partner_data, partner_size, merged);
                free(local); free(partner_data);
                local = merged; local_n += partner_size;
            }
        } else {
            int partner = rank - step;
            MPI_Send(&local_n, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            MPI_Send(local, local_n, MPI_INT, partner, 0, MPI_COMM_WORLD);
            break;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    double end = MPI_Wtime();
    if (rank == 0) {
        fprintf(stderr, "MPI Quick Sort Time (after warm-up): %f sec\n", end - start);
        if (test_mode)
            for (int i = 0; i < local_n; i++) printf("%d\n", local[i]);
    }

    free(local); if (rank == 0) free(full);
    MPI_Finalize();
    return 0;
}

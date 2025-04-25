#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s <array_size> <output_file>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    const char *filename = argv[2];

    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    srand(42); // consistent seed
    for (int i = 0; i < n; i++)
        fprintf(f, "%d\n", rand() % 10000 + 1);

    fclose(f);
    return 0;
}
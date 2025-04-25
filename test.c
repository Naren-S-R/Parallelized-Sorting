// test.c
// Runs a given binary with an input file and verifies if the result is sorted

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_sorted(const char *output_file, int n) {
    FILE *f = fopen(output_file, "r");
    if (!f) {
        perror("Failed to open result file");
        return 0;
    }
    int prev, curr;
    if (fscanf(f, "%d", &prev) != 1) {
        fclose(f);
        return 0;
    }
    for (int i = 1; i < n; i++) {
        if (fscanf(f, "%d", &curr) != 1) {
            fclose(f);
            return 0;
        }
        if (curr < prev) {
            fclose(f);
            return 0;
        }
        prev = curr;
    }
    fclose(f);
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <program_name> <array_size> <input_file>\n", argv[0]);
        return 1;
    }

    char *program = argv[1];
    char *size = argv[2];
    char *input = argv[3];

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./%s %s %s --test > temp_output.txt", program, size, input);
    int ret = system(cmd);

    if (ret != 0) {
        printf("Error running %s\n", program);
        return 1;
    }

    int sorted = is_sorted("temp_output.txt", atoi(size));
    if (sorted)
        printf("[PASS] %s sorted the array correctly.\n", program);
    else
        printf("[FAIL] %s did NOT sort the array correctly.\n", program);

    return 0;
}

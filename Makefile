# Program names
PROGRAMS = generate_input sequential_merge sequential_quick openmp_merge openmp_quick mpi_merge mpi_quick

# Compilers
CC_SEQ = gcc
CC_OMP = clang
CC_MPI = mpicc

# Compiler flags
CFLAGS = -O2 -Wall
OMP_FLAGS = -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp

# Default input size and file
ARGS ?= 1000
INPUT ?= input.txt

.PHONY: all clean run

# Build all targets
all: $(PROGRAMS)

# Build rules
generate_input: generate_input.c
	$(CC_SEQ) $(CFLAGS) -o $@ $<

sequential_%: sequential_%.c
	$(CC_SEQ) $(CFLAGS) -o $@ $<

openmp_%: openmp_%.c
	$(CC_OMP) $(CFLAGS) $(OMP_FLAGS) -o $@ $<

mpi_%: mpi_%.c
	$(CC_MPI) $(CFLAGS) -o $@ $<

# Clean up all binaries
clean:
	rm -f $(PROGRAMS)

# Run specific or all programs
run:
	@if [ "$(NAME)" = "all" ]; then \
		for prog in sequential_merge sequential_quick openmp_merge openmp_quick mpi_merge mpi_quick; do \
			echo "Running $$prog with $(ARGS) from $(INPUT)"; \
			./$$prog $(ARGS) $(INPUT); \
		done; \
	elif [ -n "$(NAME)" ]; then \
		echo "Running $(NAME) with $(ARGS) from $(INPUT)"; \
		./$(NAME) $(ARGS) $(INPUT); \
	else \
		echo "Please specify NAME=<program> or NAME=all"; \
	fi

.PHONY: run-test

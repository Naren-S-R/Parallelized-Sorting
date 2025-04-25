import matplotlib.pyplot as plt

# Input sizes used
input_sizes = [10000, 100000, 1000000, 10000000, 100000000]

# Number of threads/processes used (assumed fixed across runs)
threads_openmp = 4
procs_mpi = 4

# Replace these with your actual values
sequential_merge = [0.001713, 0.013989, 0.096720, 1.046827, 11.103669]
sequential_quick = [0.000666, 0.005758, 0.064501, 2.228275, 181.424870]
openmp_merge     = [0.000690, 0.003547, 0.032811, 0.333276, 3.586203]
openmp_quick     = [0.000751, 0.003191, 0.032031, 1.304999, 62.649521]
mpi_merge        = [0.000502, 0.004471, 0.041358, 0.447858, 4.520560]
mpi_quick        = [0.000030, 0.001495, 0.012873, 0.126554, 1.383133]

# Grouped for convenience
all_times = [
    sequential_merge,
    sequential_quick,
    openmp_merge,
    openmp_quick,
    mpi_merge,
    mpi_quick
]

program_names = [
    "Sequential Merge",
    "Sequential Quick",
    "OpenMP Merge",
    "OpenMP Quick",
    "MPI Merge",
    "MPI Quick"
]

# --------------- Metrics ---------------

# Speedup (vs sequential)
speedup_merge = [sequential_merge[i] / openmp_merge[i] for i in range(len(input_sizes))]
speedup_quick = [sequential_quick[i] / openmp_quick[i] for i in range(len(input_sizes))]
speedup_mpi_merge = [sequential_merge[i] / mpi_merge[i] for i in range(len(input_sizes))]
speedup_mpi_quick = [sequential_quick[i] / mpi_quick[i] for i in range(len(input_sizes))]

# Efficiency = Speedup / #threads or processes
eff_omp_merge = [s / threads_openmp for s in speedup_merge]
eff_omp_quick = [s / threads_openmp for s in speedup_quick]
eff_mpi_merge = [s / procs_mpi for s in speedup_mpi_merge]
eff_mpi_quick = [s / procs_mpi for s in speedup_mpi_quick]

# --------------- Plots ---------------

def plot_execution_time():
    import numpy as np
    x = np.arange(len(input_sizes))
    width = 0.13

    fig, ax = plt.subplots()
    for i, times in enumerate(all_times):
        ax.bar(x + i * width, times, width, label=program_names[i])

    ax.set_xlabel("Input Size")
    ax.set_ylabel("Execution Time (seconds)")
    ax.set_title("Execution Time vs Input Size")
    ax.set_xticks(x + width * 2.5)
    ax.set_xticklabels([str(size) for size in input_sizes])
    ax.legend()
    plt.tight_layout()
    plt.savefig("execution_time_bar.png")
    plt.show()


def plot_speedup():
    import numpy as np
    x = np.arange(len(input_sizes))
    width = 0.2

    fig, ax = plt.subplots()
    ax.bar(x - width*1.5, speedup_merge, width, label="OMP Merge")
    ax.bar(x - width/2, speedup_quick, width, label="OMP Quick")
    ax.bar(x + width/2, speedup_mpi_merge, width, label="MPI Merge")
    ax.bar(x + width*1.5, speedup_mpi_quick, width, label="MPI Quick")

    ax.set_xlabel("Input Size")
    ax.set_ylabel("Speedup")
    ax.set_title("Speedup vs Input Size")
    ax.set_xticks(x)
    ax.set_xticklabels([str(size) for size in input_sizes])
    ax.legend()
    plt.tight_layout()
    plt.savefig("speedup_bar.png")
    plt.show()


def plot_efficiency():
    import numpy as np
    x = np.arange(len(input_sizes))
    width = 0.2

    fig, ax = plt.subplots()
    ax.bar(x - width*1.5, eff_omp_merge, width, label="OMP Merge")
    ax.bar(x - width/2, eff_omp_quick, width, label="OMP Quick")
    ax.bar(x + width/2, eff_mpi_merge, width, label="MPI Merge")
    ax.bar(x + width*1.5, eff_mpi_quick, width, label="MPI Quick")

    ax.set_xlabel("Input Size")
    ax.set_ylabel("Efficiency")
    ax.set_title("Efficiency vs Input Size")
    ax.set_xticks(x)
    ax.set_xticklabels([str(size) for size in input_sizes])
    ax.legend()
    plt.tight_layout()
    plt.savefig("efficiency_bar.png")
    plt.show()


# --------------- Summary Table ---------------

import csv

def save_to_csv(filename="metrics.csv"):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        
        # Header row
        writer.writerow([
            "InputSize",
            "SequentialMerge", "SequentialQuick",
            "OpenMPMerge", "OpenMPQuick",
            "MPIMerge", "MPIQuick",
            "OMPMergeSpeedup", "OMPQuickSpeedup",
            "MPIMergeSpeedup", "MPIQuickSpeedup",
            "OMPMergeEfficiency", "OMPQuickEfficiency",
            "MPIMergeEfficiency", "MPIQuickEfficiency"
        ])
        
        for i in range(len(input_sizes)):
            writer.writerow([
                input_sizes[i],
                sequential_merge[i], sequential_quick[i],
                openmp_merge[i], openmp_quick[i],
                mpi_merge[i], mpi_quick[i],
                speedup_merge[i], speedup_quick[i],
                speedup_mpi_merge[i], speedup_mpi_quick[i],
                eff_omp_merge[i], eff_omp_quick[i],
                eff_mpi_merge[i], eff_mpi_quick[i]
            ])
    print(f"\nCSV saved to {filename}")

if __name__ == "__main__":
    plot_execution_time()
    plot_speedup()
    plot_efficiency()
    save_to_csv()  
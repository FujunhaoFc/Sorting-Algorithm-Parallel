# Parallel Sorting Algorithms Comparison

This project implements and compares different sorting algorithms in both sequential and parallel versions using OpenMP. The implemented algorithms include Merge Sort, Quick Sort, Heap Sort, and Radix Sort.

## Project Structure

```
project_root/
├── CMakeLists.txt
├── include/
│   ├── SortingAlgorithm.hpp
│   ├── MergeSort.hpp
│   ├── QuickSort.hpp
│   ├── HeapSort.hpp
│   ├── RadixSort.hpp
│   └── Benchmark.hpp
├── src/
│   ├── MergeSort.cpp
│   ├── QuickSort.cpp
│   ├── HeapSort.cpp
│   ├── RadixSort.cpp
│   ├── Benchmark.cpp
│   └── main.cpp
└── README.md
```

## Prerequisites

- C++ Compiler with C++17 support (GCC 7+ or Clang 5+)
- CMake (version 3.10 or higher)
- OpenMP library
- Make

### Installing Prerequisites

For Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libomp-dev
```

For CentOS/RHEL:
```bash
sudo yum install gcc gcc-c++ cmake make openmp
```


## Building the Project

1. Clone the repository:
```bash
git clone <repository-url>
cd <repository-name>
```

2. Create and enter the build directory:
```bash
mkdir build
cd build
```

3. Generate build files and compile:
```bash
cmake ..
make
```

## Running the Program

After building, run the executable:
```bash
./ParallelSorting
```

The program will:
- Run benchmarks for all sorting algorithms
- Test with different input sizes
- Compare sequential and parallel implementations
- Generate a CSV file with the results

## Benchmark Results

The program generates a CSV file named `sorting_results.csv` containing:
- Algorithm name
- Input size
- Data type (random, nearly sorted, etc.)
- Execution time
- Number of threads used
- Speedup achieved
- Correctness verification

## Implementation Details

### Supported Algorithms

1. Merge Sort
   - Sequential implementation
   - Parallel implementation using divide-and-conquer

2. Quick Sort
   - Sequential implementation
   - Parallel implementation with parallel partitioning

3. Heap Sort
   - Sequential implementation
   - Parallel implementation with parallel heap building

4. Radix Sort
   - Sequential implementation
   - Parallel implementation with parallel counting

### Parallel Implementation Features

- OpenMP parallel sections for divide-and-conquer algorithms
- Thread-safe operations using atomic directives
- Dynamic load balancing
- Cache-friendly memory access patterns

## Performance Analysis

The benchmark system measures:
- Execution time
- Speedup relative to sequential version
- Scaling efficiency with different thread counts
- Performance with different input sizes
- Performance with different input distributions

## Troubleshooting

1. If CMake can't find OpenMP:
```bash
# For Ubuntu/Debian
sudo apt-get install libomp-dev


2. If the build fails with compiler errors:
```bash
# Clean the build directory
rm -rf build/*
# Rebuild with verbose output
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
make
```

3. If the program doesn't use multiple threads:
```bash
# Set the number of threads manually
export OMP_NUM_THREADS=4
```



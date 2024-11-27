{\rtf1\ansi\ansicpg936\cocoartf2820
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 Parallel Sorting Algorithms Comparison\
\
This project implements and compares different sorting algorithms in both sequential and parallel versions using OpenMP. The implemented algorithms include Merge Sort, Quick Sort, Heap Sort, and Radix Sort.\
\
Project Structure:\
project_root/\
\uc0\u9500 \u9472 \u9472  CMakeLists.txt\
\uc0\u9500 \u9472 \u9472  include/\
\uc0\u9474    \u9500 \u9472 \u9472  SortingAlgorithm.hpp\
\uc0\u9474    \u9500 \u9472 \u9472  MergeSort.hpp\
\uc0\u9474    \u9500 \u9472 \u9472  QuickSort.hpp\
\uc0\u9474    \u9500 \u9472 \u9472  HeapSort.hpp\
\uc0\u9474    \u9500 \u9472 \u9472  RadixSort.hpp\
\uc0\u9474    \u9492 \u9472 \u9472  Benchmark.hpp\
\uc0\u9500 \u9472 \u9472  src/\
\uc0\u9474    \u9500 \u9472 \u9472  MergeSort.cpp\
\uc0\u9474    \u9500 \u9472 \u9472  QuickSort.cpp\
\uc0\u9474    \u9500 \u9472 \u9472  HeapSort.cpp\
\uc0\u9474    \u9500 \u9472 \u9472  RadixSort.cpp\
\uc0\u9474    \u9500 \u9472 \u9472  Benchmark.cpp\
\uc0\u9474    \u9492 \u9472 \u9472  main.cpp\
\uc0\u9492 \u9472 \u9472  README.md\
\
Prerequisites:\
- C++ Compiler with C++17 support (GCC 7+ or Clang 5+)\
- CMake (version 3.10 or higher)\
- OpenMP library\
- Make\
\
Installing Prerequisites:\
For Ubuntu/Debian:\
sudo apt-get update\
sudo apt-get install build-essential cmake libomp-dev\
\
For CentOS/RHEL:\
sudo yum install gcc gcc-c++ cmake make openmp\
\
Building the Project:\
1. Clone the repository:\
git clone <repository-url>\
cd <repository-name>\
\
2. Create and enter the build directory:\
mkdir build\
cd build\
\
3. Generate build files and compile:\
cmake ..\
make\
\
Running the Program:\
After building, run the executable:\
./ParallelSorting\
\
The program will:\
- Run benchmarks for all sorting algorithms\
- Test with different input sizes\
- Compare sequential and parallel implementations\
- Generate a CSV file with the results\
\
Benchmark Results:\
The program generates a CSV file named 'sorting_results.csv' containing:\
- Algorithm name\
- Input size\
- Data type (random, nearly sorted, etc.)\
- Execution time\
- Number of threads used\
- Speedup achieved\
- Correctness verification\
\
Implementation Details:\
\
Supported Algorithms:\
1. Merge Sort\
   - Sequential implementation\
   - Parallel implementation using divide-and-conquer\
\
2. Quick Sort\
   - Sequential implementation\
   - Parallel implementation with parallel partitioning\
\
3. Heap Sort\
   - Sequential implementation\
   - Parallel implementation with parallel heap building\
\
4. Radix Sort\
   - Sequential implementation\
   - Parallel implementation with parallel counting\
\
Parallel Implementation Features:\
- OpenMP parallel sections for divide-and-conquer algorithms\
- Thread-safe operations using atomic directives\
- Dynamic load balancing\
- Cache-friendly memory access patterns\
\
Performance Analysis:\
The benchmark system measures:\
- Execution time\
- Speedup relative to sequential version\
- Scaling efficiency with different thread counts\
- Performance with different input sizes\
- Performance with different input distributions\
\
Troubleshooting:\
1. If CMake can't find OpenMP:\
For Ubuntu/Debian:\
sudo apt-get install libomp-dev\
\
2. If the build fails with compiler errors:\
# Clean the build directory\
rm -rf build/*\
# Rebuild with verbose output\
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..\
make\
\
3. If the program doesn't use multiple threads:\
# Set the number of threads manually\
export OMP_NUM_THREADS=4}
#include "RadixSort.hpp"
#include <omp.h>
#include <algorithm>
#include <vector>
#include <chrono>
#include <iostream>

// Global timing counters in milliseconds
double time_spent_on_counting = 0;
double time_spent_on_cumulative_sum = 0;
double time_spent_on_output_construction = 0;

// Sequential RadixSort Implementation
SequentialRadixSort::SequentialRadixSort() : SortingAlgorithm("Sequential Radix Sort", false) {}

int SequentialRadixSort::getMax(const std::vector<int>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

void SequentialRadixSort::countSort(std::vector<int>& arr, int exp) {
    std::vector<int> output(arr.size());
    int count[RADIX] = {0};

    // Count occurrences of each digit
    auto start_counting = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < arr.size(); i++) {
        count[(arr[i] / exp) % RADIX]++;
    }
    auto end_counting = std::chrono::high_resolution_clock::now();
    time_spent_on_counting += std::chrono::duration<double, std::milli>(end_counting - start_counting).count();

    // Calculate cumulative count
    auto start_cumulative = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < RADIX; i++) {
        count[i] += count[i - 1];
    }
    auto end_cumulative = std::chrono::high_resolution_clock::now();
    time_spent_on_cumulative_sum += std::chrono::duration<double, std::milli>(end_cumulative - start_cumulative).count();

    // Build output array
    auto start_output = std::chrono::high_resolution_clock::now();
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[--count[(arr[i] / exp) % RADIX]] = arr[i];
    }
    auto end_output = std::chrono::high_resolution_clock::now();
    time_spent_on_output_construction += std::chrono::duration<double, std::milli>(end_output - start_output).count();

    // Copy output back to arr
    arr = output;
}

void SequentialRadixSort::sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;

    int maxVal = getMax(arr);
    for (int exp = 1; maxVal / exp > 0; exp *= RADIX) {
        countSort(arr, exp);
    }

    // Print timing information
    std::cout << "[Sequential] Time spent on counting: " << time_spent_on_counting << " ms\n";
    std::cout << "[Sequential] Time spent on cumulative sum: " << time_spent_on_cumulative_sum << " ms\n";
    std::cout << "[Sequential] Time spent on output construction: " << time_spent_on_output_construction << " ms\n";
}

// Parallel RadixSort Implementation
ParallelRadixSort::ParallelRadixSort() : SortingAlgorithm("Parallel Radix Sort", true) {}

int ParallelRadixSort::getMax(const std::vector<int>& arr) {
    int max_val = arr[0];
    #pragma omp parallel for reduction(max:max_val)
    for (size_t i = 1; i < arr.size(); i++) {
        max_val = std::max(max_val, arr[i]);
    }
    return max_val;
}

void ParallelRadixSort::parallelCountSort(std::vector<int>& source, std::vector<int>& destination, int exp) {
    const int n = source.size();
    std::vector<int> count(RADIX, 0);

    // Step 1: Parallel counting
    auto start_counting = std::chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        std::vector<int> local_count(RADIX, 0);

        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++) {
            int digit = (source[i] / exp) % RADIX;
            local_count[digit]++;
        }

        #pragma omp critical
        {
            for (int i = 0; i < RADIX; i++) {
                count[i] += local_count[i];
            }
        }
    }
    auto end_counting = std::chrono::high_resolution_clock::now();
    time_spent_on_counting += std::chrono::duration<double, std::milli>(end_counting - start_counting).count();

    // Step 2: Cumulative sum (sequential)
    auto start_cumulative = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < RADIX; i++) {
        count[i] += count[i - 1];
    }
    auto end_cumulative = std::chrono::high_resolution_clock::now();
    time_spent_on_cumulative_sum += std::chrono::duration<double, std::milli>(end_cumulative - start_cumulative).count();

    // Step 3: Build output array using thread-local ranges
    auto start_output = std::chrono::high_resolution_clock::now();
    std::vector<int> thread_offsets(RADIX * omp_get_max_threads(), 0);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        std::vector<int> local_offsets(RADIX, 0);

        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++) {
            int digit = (source[i] / exp) % RADIX;
            local_offsets[digit]++;
        }

        // Prefix sum for thread-local ranges
        int global_offset = 0;
        for (int i = 0; i < RADIX; i++) {
            thread_offsets[thread_id * RADIX + i] = global_offset;
            global_offset += local_offsets[i];
        }

        #pragma omp barrier

        // Aggregate thread-local offsets into global offsets
        #pragma omp single
        for (int i = 0; i < RADIX; i++) {
            int total = 0;
            for (int j = 0; j < omp_get_max_threads(); j++) {
                int temp = thread_offsets[j * RADIX + i];
                thread_offsets[j * RADIX + i] = total;
                total += temp;
            }
        }

        #pragma omp barrier

        // Write output based on thread-local offsets
        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++) {
            int digit = (source[i] / exp) % RADIX;
            int position = thread_offsets[thread_id * RADIX + digit]++;
            destination[position] = source[i];
        }
    }
    auto end_output = std::chrono::high_resolution_clock::now();
    time_spent_on_output_construction += std::chrono::duration<double, std::milli>(end_output - start_output).count();
}

void ParallelRadixSort::sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;

    const int THRESHOLD = 10000; // Switch to sequential for small datasets
    if (arr.size() <= THRESHOLD) {
        SequentialRadixSort().sort(arr);
        return;
    }

    std::vector<int> output(arr.size());
    int maxVal = getMax(arr);

    time_spent_on_counting = 0;
    time_spent_on_cumulative_sum = 0;
    time_spent_on_output_construction = 0;

    for (int exp = 1, pass = 0; maxVal / exp > 0; exp *= RADIX, pass++) {
        if (pass % 2 == 0) {
            parallelCountSort(arr, output, exp);
        } else {
            parallelCountSort(output, arr, exp);
        }
    }

    // Ensure final result is in arr
    if ((BITS / 8) % 2 != 0) {
        std::copy(output.begin(), output.end(), arr.begin());
    }

    // Print timing information
    std::cout << "[Parallel] Time spent on counting: " << time_spent_on_counting << " ms\n";
    std::cout << "[Parallel] Time spent on cumulative sum: " << time_spent_on_cumulative_sum << " ms\n";
    std::cout << "[Parallel] Time spent on output construction: " << time_spent_on_output_construction << " ms\n";
}

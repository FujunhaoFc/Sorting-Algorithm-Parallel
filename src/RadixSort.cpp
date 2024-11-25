#include "RadixSort.hpp"
#include <omp.h>
#include <algorithm>
#include <cstring>

// Sequential RadixSort Implementation
SequentialRadixSort::SequentialRadixSort() : SortingAlgorithm("Sequential Radix Sort", false) {}

int SequentialRadixSort::getMax(const std::vector<int>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

void SequentialRadixSort::countSort(std::vector<int>& arr, int exp) {
    std::vector<int> output(arr.size());
    int count[256] = {0}; // Using 8 bits at a time

    // Store count of occurrences
    for(size_t i = 0; i < arr.size(); i++)
        count[(arr[i] / exp) & 0xFF]++;

    // Change count[i] so that count[i] contains actual
    // position of this digit in output[]
    for(int i = 1; i < 256; i++)
        count[i] += count[i - 1];

    // Build the output array
    for(int i = arr.size() - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) & 0xFF] - 1] = arr[i];
        count[(arr[i] / exp) & 0xFF]--;
    }

    // Copy the output array to arr[]
    for(size_t i = 0; i < arr.size(); i++)
        arr[i] = output[i];
}

void SequentialRadixSort::sort(std::vector<int>& arr) {
    // Handle empty or single-element array
    if (arr.size() <= 1) return;

    // Do counting sort for every byte (8 bits)
    // Instead of finding max element, we'll process all 32 bits
    for (int exp = 0; exp < 32; exp += 8) {
        countSort(arr, 1 << exp);
    }
}

// Parallel RadixSort Implementation
ParallelRadixSort::ParallelRadixSort() : SortingAlgorithm("Parallel Radix Sort", true) {}

int ParallelRadixSort::getMax(const std::vector<int>& arr) {
    int max_val = arr[0];
    #pragma omp parallel for reduction(max:max_val)
    for(size_t i = 1; i < arr.size(); i++) {
        max_val = std::max(max_val, arr[i]);
    }
    return max_val;
}

void ParallelRadixSort::parallelCountSort(std::vector<int>& arr, int exp) {
    const int n = arr.size();
    std::vector<int> output(n);
    
    // Use per-thread private counts to avoid false sharing
    int num_threads;
    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }
    
    std::vector<std::vector<int>> thread_counts(num_threads, std::vector<int>(RADIX, 0));
    
    // Count occurrences in parallel
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        auto& local_count = thread_counts[thread_id];
        
        #pragma omp for schedule(static)
        for(int i = 0; i < n; i++) {
            local_count[(arr[i] >> exp) & (RADIX - 1)]++;
        }
    }
    
    // Combine counts from all threads
    std::vector<int> count(RADIX, 0);
    for(int i = 0; i < num_threads; i++) {
        for(int j = 0; j < RADIX; j++) {
            count[j] += thread_counts[i][j];
        }
    }
    
    // Calculate cumulative sum
    for(int i = 1; i < RADIX; i++) {
        count[i] += count[i - 1];
    }
    
    // Build output array in parallel
    std::vector<int> pos(RADIX);
    std::copy(count.begin(), count.end(), pos.begin());
    
    #pragma omp parallel for schedule(static)
    for(int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] >> exp) & (RADIX - 1);
        int position;
        
        #pragma omp atomic capture
        position = --pos[digit];
        
        
        output[position] = arr[i];
    }
    
    // Copy back to original array
    #pragma omp parallel for schedule(static)
    for(int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void ParallelRadixSort::sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;

    // Process 8 bits at a time
    for(int bit = 0; bit < BITS; bit += 8) {
        parallelCountSort(arr, bit);
    }
}
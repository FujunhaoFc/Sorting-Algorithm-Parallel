#pragma once
#include "SortingAlgorithm.hpp"
#include <vector>

// Shared constants for Radix Sort
constexpr int RADIX = 256;  // Using 8 bits at a time
constexpr int BITS = 32;    // Assuming 32-bit integers

class SequentialRadixSort : public SortingAlgorithm {
private:
    void countSort(std::vector<int>& arr, int exp);
    int getMax(const std::vector<int>& arr);

public:
    SequentialRadixSort();
    void sort(std::vector<int>& arr) override;
};

class ParallelRadixSort : public SortingAlgorithm {
private:
    // Modified to take separate source and destination arrays
    void parallelCountSort(std::vector<int>& source, std::vector<int>& destination, int exp);
    int getMax(const std::vector<int>& arr);

public:
    ParallelRadixSort();
    void sort(std::vector<int>& arr) override;
};

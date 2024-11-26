#pragma once
#include "SortingAlgorithm.hpp"
#include <vector>

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
    static const int RADIX = 256;  // Using 8 bits at a time
    static const int BITS = 32;    // Assuming 32-bit integers

    // Modified to take separate source and destination arrays
    void parallelCountSort(std::vector<int>& source, std::vector<int>& destination, int exp);
    int getMax(const std::vector<int>& arr);

public:
    ParallelRadixSort();
    void sort(std::vector<int>& arr) override;
};

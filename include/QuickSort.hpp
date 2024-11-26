#pragma once
#include "SortingAlgorithm.hpp"

class SequentialQuickSort : public SortingAlgorithm {
private:
    int partition(std::vector<int>& arr, int low, int high);

public:
    SequentialQuickSort();
    void quickSort(std::vector<int>& arr, int low, int high); // Made public
    void sort(std::vector<int>& arr) override;
};

class ParallelQuickSort : public SortingAlgorithm {
private:
    int partition(std::vector<int>& arr, int low, int high);
    void parallelQuickSort(std::vector<int>& arr, int low, int high);

public:
    ParallelQuickSort();
    void sort(std::vector<int>& arr) override;
};

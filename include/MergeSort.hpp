#pragma once
#include "SortingAlgorithm.hpp"

class SequentialMergeSort : public SortingAlgorithm {
private:
    void merge(std::vector<int>& arr, int left, int mid, int right);
    void mergeSort(std::vector<int>& arr, int left, int right);

public:
    SequentialMergeSort();
    void sort(std::vector<int>& arr) override;
};

class ParallelMergeSort : public SortingAlgorithm {
private:
    void merge(std::vector<int>& arr, int left, int mid, int right);
    void parallelMergeSort(std::vector<int>& arr, int left, int right, int depth = 0);

public:
    ParallelMergeSort();
    void sort(std::vector<int>& arr) override;
};

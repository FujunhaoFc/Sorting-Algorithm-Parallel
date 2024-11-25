#pragma once
#include "SortingAlgorithm.hpp"

class SequentialHeapSort : public SortingAlgorithm {
private:
    void heapify(std::vector<int>& arr, int n, int i);

public:
    SequentialHeapSort();
    void sort(std::vector<int>& arr) override;
};

class ParallelHeapSort : public SortingAlgorithm {
private:
    void heapify(std::vector<int>& arr, int n, int i);
    void parallelBuildHeap(std::vector<int>& arr);

public:
    ParallelHeapSort();
    void sort(std::vector<int>& arr) override;
};
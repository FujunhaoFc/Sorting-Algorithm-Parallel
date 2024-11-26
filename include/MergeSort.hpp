#pragma once
#include "SortingAlgorithm.hpp"
#include <vector>

class SequentialMergeSort : public SortingAlgorithm {
private:
    std::vector<int> aux;
    void merge(std::vector<int>& arr, int left, int mid, int right);
    void mergeSort(std::vector<int>& arr, int left, int right);
    void insertionSort(std::vector<int>& arr, int left, int right);

public:
    SequentialMergeSort();
    void sort(std::vector<int>& arr) override;
};

class ParallelMergeSort : public SortingAlgorithm {
private:
    void merge(std::vector<int>& arr, int left, int mid, int right, std::vector<int>& aux);
    void parallelMergeSort(std::vector<int>& arr, int left, int right);
    void insertionSort(std::vector<int>& arr, int left, int right);

public:
    ParallelMergeSort();
    void sort(std::vector<int>& arr) override;
};
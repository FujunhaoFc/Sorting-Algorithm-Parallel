#include "QuickSort.hpp"
#include <omp.h>

SequentialQuickSort::SequentialQuickSort() : SortingAlgorithm("Sequential Quick Sort", false) {}

int SequentialQuickSort::partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void SequentialQuickSort::quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void SequentialQuickSort::sort(std::vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

ParallelQuickSort::ParallelQuickSort() : SortingAlgorithm("Parallel Quick Sort", true) {}

int ParallelQuickSort::partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void ParallelQuickSort::parallelQuickSort(std::vector<int>& arr, int low, int high, int depth) {
    if (low < high) {
        if (depth >= omp_get_max_threads() || (high - low) <= 10000) {
            int pi = partition(arr, low, high);
            parallelQuickSort(arr, low, pi - 1, depth + 1);
            parallelQuickSort(arr, pi + 1, high, depth + 1);
            return;
        }

        int pi = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelQuickSort(arr, low, pi - 1, depth + 1);
            }
            #pragma omp section
            {
                parallelQuickSort(arr, pi + 1, high, depth + 1);
            }
        }
    }
}

void ParallelQuickSort::sort(std::vector<int>& arr) {
    parallelQuickSort(arr, 0, arr.size() - 1);
}
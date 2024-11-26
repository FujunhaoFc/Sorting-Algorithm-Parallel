#include "QuickSort.hpp"
#include <omp.h>

#define THRESHOLD 8000 // Adjust this threshold based on experimentation

SequentialQuickSort::SequentialQuickSort() : SortingAlgorithm("Sequential Quick Sort", false) {}

int SequentialQuickSort::partition(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    int pivot = arr[mid];

    // Move pivot to the end
    std::swap(arr[mid], arr[high]);

    int i = low;

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }

    // Move pivot to its final place
    std::swap(arr[i], arr[high]);

    return i;
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

void ParallelQuickSort::parallelQuickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        if ((high - low) <= THRESHOLD) {
            SequentialQuickSort().quickSort(arr, low, high);
            return;
        }

        int pi = partition(arr, low, high);

        #pragma omp task shared(arr)
        {
            parallelQuickSort(arr, low, pi - 1);
        }

        #pragma omp task shared(arr)
        {
            parallelQuickSort(arr, pi + 1, high);
        }

        #pragma omp taskwait
    }
}

void ParallelQuickSort::sort(std::vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            parallelQuickSort(arr, 0, arr.size() - 1);
        }
    }
}

#include "HeapSort.hpp"
#include <omp.h>

SequentialHeapSort::SequentialHeapSort() : SortingAlgorithm("Sequential Heap Sort", false) {}

void SequentialHeapSort::heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void SequentialHeapSort::sort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

ParallelHeapSort::ParallelHeapSort() : SortingAlgorithm("Parallel Heap Sort", true) {}

void ParallelHeapSort::heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void ParallelHeapSort::parallelBuildHeap(std::vector<int>& arr) {
    int n = arr.size();
    #pragma omp parallel for
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}

void ParallelHeapSort::sort(std::vector<int>& arr) {
    int n = arr.size();
    
    parallelBuildHeap(arr);

    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
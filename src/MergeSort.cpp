#include "MergeSort.hpp"
#include <omp.h>

#define INSERTION_SORT_THRESHOLD 16
#define PARALLEL_THRESHOLD 10000

// SequentialMergeSort implementation

SequentialMergeSort::SequentialMergeSort() : SortingAlgorithm("Sequential Merge Sort", false) {}

void SequentialMergeSort::insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void SequentialMergeSort::merge(std::vector<int>& arr, int left, int mid, int right) {
    int i = left, j = mid + 1;

    // Copy the relevant portion to aux
    for (int k = left; k <= right; ++k) {
        aux[k] = arr[k];
    }

    for (int k = left; k <= right; ++k) {
        if (i > mid) {
            arr[k] = aux[j++];
        } else if (j > right) {
            arr[k] = aux[i++];
        } else if (aux[i] <= aux[j]) {
            arr[k] = aux[i++];
        } else {
            arr[k] = aux[j++];
        }
    }
}

void SequentialMergeSort::mergeSort(std::vector<int>& arr, int left, int right) {
    if (right - left + 1 <= INSERTION_SORT_THRESHOLD) {
        insertionSort(arr, left, right);
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void SequentialMergeSort::sort(std::vector<int>& arr) {
    aux.resize(arr.size());
    mergeSort(arr, 0, arr.size() - 1);
}

// ParallelMergeSort implementation

ParallelMergeSort::ParallelMergeSort() : SortingAlgorithm("Parallel Merge Sort", true) {}

void ParallelMergeSort::insertionSort(std::vector<int>& arr, int left, int right) {
    // Same as in SequentialMergeSort
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void ParallelMergeSort::merge(std::vector<int>& arr, int left, int mid, int right, std::vector<int>& aux) {
    int i = left, j = mid + 1, k = left;

    // Copy data to auxiliary array
    for (int idx = left; idx <= right; ++idx) {
        aux[idx] = arr[idx];
    }

    while (i <= mid && j <= right) {
        if (aux[i] <= aux[j]) {
            arr[k++] = aux[i++];
        } else {
            arr[k++] = aux[j++];
        }
    }

    while (i <= mid) {
        arr[k++] = aux[i++];
    }

    while (j <= right) {
        arr[k++] = aux[j++];
    }
}

void ParallelMergeSort::parallelMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int n = right - left + 1;
        if (n <= INSERTION_SORT_THRESHOLD) {
            insertionSort(arr, left, right);
            return;
        }

        int mid = left + (right - left) / 2;

        if (n > PARALLEL_THRESHOLD) {
            #pragma omp task shared(arr)
            {
                parallelMergeSort(arr, left, mid);
            }

            #pragma omp task shared(arr)
            {
                parallelMergeSort(arr, mid + 1, right);
            }

            #pragma omp taskwait
        } else {
            parallelMergeSort(arr, left, mid);
            parallelMergeSort(arr, mid + 1, right);
        }

        // Each thread should have its own aux array
        std::vector<int> aux(arr.size());
        merge(arr, left, mid, right, aux);
    }
}

void ParallelMergeSort::sort(std::vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            parallelMergeSort(arr, 0, arr.size() - 1);
        }
    }
}
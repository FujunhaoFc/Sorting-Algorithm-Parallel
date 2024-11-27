#include "MergeSort.hpp"
#include <omp.h>

// SequentialMergeSort implementation
SequentialMergeSort::SequentialMergeSort() : SortingAlgorithm("Sequential Merge Sort", false) {}

void SequentialMergeSort::merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (i = 0; i < k; i++) arr[left + i] = temp[i];
}

void SequentialMergeSort::mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void SequentialMergeSort::sort(std::vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

// ParallelMergeSort implementation
ParallelMergeSort::ParallelMergeSort() : SortingAlgorithm("Parallel Merge Sort", true) {}

void ParallelMergeSort::merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (i = 0; i < k; i++) arr[left + i] = temp[i];
}

void ParallelMergeSort::parallelMergeSort(std::vector<int>& arr, int left, int right, int depth) {
    if (left < right) {
        if (depth >= omp_get_max_threads() || (right - left) <= 10000) {
            int mid = left + (right - left) / 2;
            parallelMergeSort(arr, left, mid, depth + 1);
            parallelMergeSort(arr, mid + 1, right, depth + 1);
            merge(arr, left, mid, right);
            return;
        }

        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, left, mid, depth + 1);
            }
            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, right, depth + 1);
            }
        }
        merge(arr, left, mid, right);
    }
}

void ParallelMergeSort::sort(std::vector<int>& arr) {
    parallelMergeSort(arr, 0, arr.size() - 1);
}

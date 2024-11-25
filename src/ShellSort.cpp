#include "ShellSort.hpp"
#include <omp.h>

SequentialShellSort::SequentialShellSort() : SortingAlgorithm("Sequential Shell Sort", false) {}

std::vector<int> SequentialShellSort::generateGaps(int n) {
    std::vector<int> gaps;
    int gap = n;
    while (gap > 0) {
        gap = gap / 2;
        if (gap > 0) gaps.push_back(gap);
    }
    return gaps;
}

void SequentialShellSort::sort(std::vector<int>& arr) {
    int n = arr.size();
    auto gaps = generateGaps(n);

    for (int gap : gaps) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

ParallelShellSort::ParallelShellSort() : SortingAlgorithm("Parallel Shell Sort", true) {}

std::vector<int> ParallelShellSort::generateGaps(int n) {
    std::vector<int> gaps;
    int gap = n;
    while (gap > 0) {
        gap = gap / 2;
        if (gap > 0) gaps.push_back(gap);
    }
    return gaps;
}

void ParallelShellSort::sort(std::vector<int>& arr) {
    int n = arr.size();
    auto gaps = generateGaps(n);

    for (int gap : gaps) {
        #pragma omp parallel for schedule(dynamic)
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}
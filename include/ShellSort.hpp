#pragma once
#include "SortingAlgorithm.hpp"

class SequentialShellSort : public SortingAlgorithm {
private:
    std::vector<int> generateGaps(int n);

public:
    SequentialShellSort();
    void sort(std::vector<int>& arr) override;
};

class ParallelShellSort : public SortingAlgorithm {
private:
    std::vector<int> generateGaps(int n);

public:
    ParallelShellSort();
    void sort(std::vector<int>& arr) override;
};
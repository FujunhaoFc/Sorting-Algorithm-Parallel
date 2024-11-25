#pragma once
#include <vector>
#include <string>

class SortingAlgorithm {
protected:
    std::string name;
    bool is_parallel;

public:
    SortingAlgorithm(const std::string& name, bool is_parallel) 
        : name(name), is_parallel(is_parallel) {}
    virtual void sort(std::vector<int>& arr) = 0;
    std::string getName() const { return name; }
    bool isParallel() const { return is_parallel; }
    virtual ~SortingAlgorithm() = default;
};
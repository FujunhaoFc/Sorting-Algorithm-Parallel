#pragma once
#include "SortingAlgorithm.hpp"
#include <memory>
#include <vector>
#include <string>
#include <map>

class Benchmark {
public:
    struct BenchmarkResult {
        double avgTime;
        double minTime;
        double maxTime;
        double stdDev;
        double speedup;
        bool correctness;
        size_t dataSize;
        std::string dataType;
    };

    void addAlgorithm(std::unique_ptr<SortingAlgorithm> algo);
    void runBenchmark(const std::vector<size_t>& dataSizes, int numTests = 5);
    void saveResults(const std::string& filename) const;
    void printResults() const;

private:
    std::vector<std::unique_ptr<SortingAlgorithm>> algorithms;
    std::map<std::string, std::vector<BenchmarkResult>> results;

    std::vector<int> generateRandomData(size_t size);
    std::vector<int> generateNearlySortedData(size_t size, double sortedFraction);
    std::vector<int> generateReverseSortedData(size_t size);
    BenchmarkResult runSingleTest(SortingAlgorithm* algo, std::vector<int>& data);
};
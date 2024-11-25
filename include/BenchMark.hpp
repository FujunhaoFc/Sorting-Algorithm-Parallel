#pragma once
#include "SortingAlgorithm.hpp"
#include <memory>
#include <vector>
#include <string>
#include <map>

class Benchmark {
public:
    // Result structure for individual tests
    struct TestResult {
        double avgTime;
        double minTime;
        double maxTime;
        double stdDev;
        double speedup;
        bool correctness;
        size_t dataSize;
        std::string dataType;
        int threadCount;
    };

    // Constructor
    Benchmark();

    // Add a sorting algorithm to test
    void addAlgorithm(std::unique_ptr<SortingAlgorithm> algo);

    // Main benchmark function
    void runBenchmarks(const std::vector<size_t>& dataSizes, 
                      int numTests = 5,
                      int maxThreads = 8);

    // Results export and display
    void exportResults(const std::string& filename) const;
    void printResults() const;
    const std::map<std::string, std::vector<TestResult>>& getResults() const;

private:
    // Member variables
    std::vector<std::unique_ptr<SortingAlgorithm>> algorithms;
    std::map<std::string, std::vector<TestResult>> results;

    // Data generation methods
    std::vector<int> generateRandomData(size_t size);
    std::vector<int> generateNearlySortedData(size_t size, double sortedFraction = 0.7);
    std::vector<int> generateReverseSortedData(size_t size);
    std::vector<int> generateFewUniqueData(size_t size, int numUnique = 10);

    // Helper methods
    bool verifySorting(const std::vector<int>& data) const;
    
    TestResult runSingleTest(SortingAlgorithm* algo,
                           std::vector<int>& data,
                           const std::string& dataType,
                           size_t dataSize,
                           int threadCount);

    TestResult calculateStatistics(const std::vector<double>& times,
                                 const std::string& dataType,
                                 size_t dataSize,
                                 int threadCount,
                                 bool correctness);
};
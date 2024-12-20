#include <iostream>
#include "Benchmark.hpp"
#include "MergeSort.hpp"
#include "QuickSort.hpp"
#include "RadixSort.hpp"

int main() {
    // Create benchmark instance
    Benchmark benchmark;
    
    // Add all sorting algorithms
    benchmark.addAlgorithm(std::make_unique<SequentialMergeSort>());
    benchmark.addAlgorithm(std::make_unique<ParallelMergeSort>());
    benchmark.addAlgorithm(std::make_unique<SequentialQuickSort>());
    benchmark.addAlgorithm(std::make_unique<ParallelQuickSort>());
    benchmark.addAlgorithm(std::make_unique<SequentialRadixSort>());
    benchmark.addAlgorithm(std::make_unique<ParallelRadixSort>());
    
    // Define data sizes to test
    std::vector<size_t> dataSizes = {
        10000,      // 10K elements
        100000,     // 100K elements
	500000,     // 500K elements
        1000000,     // 1M elements
        5000000     // 5M elements
    };
    
    // Run benchmarks
    // Parameters: data sizes, number of tests per configuration, max thread count
    benchmark.runBenchmarks(dataSizes, 5, 64);
    
    // Print results to console
    benchmark.printResults();
    
    // Export results to CSV file
    benchmark.exportResults("sorting_benchmark_results.csv");
    
    return 0;
}

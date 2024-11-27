// src/Benchmark.cpp
#include "Benchmark.hpp"
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <omp.h>

Benchmark::Benchmark() {}

void Benchmark::addAlgorithm(std::unique_ptr<SortingAlgorithm> algo) {
    algorithms.push_back(std::move(algo));
}

std::vector<int> Benchmark::generateRandomData(size_t size) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    
    #pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
        data[i] = dis(gen);
    }
    return data;
}

std::vector<int> Benchmark::generateNearlySortedData(size_t size, double sortedFraction) {
    std::vector<int> data = generateRandomData(size);
    size_t sortedElements = static_cast<size_t>(size * sortedFraction);
    std::sort(data.begin(), data.begin() + sortedElements);
    return data;
}

std::vector<int> Benchmark::generateReverseSortedData(size_t size) {
    std::vector<int> data(size);
    for (size_t i = 0; i < size; i++) {
        data[i] = size - i;
    }
    return data;
}

std::vector<int> Benchmark::generateFewUniqueData(size_t size, int numUnique) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, numUnique - 1);
    
    #pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
        data[i] = dis(gen);
    }
    return data;
}

bool Benchmark::verifySorting(const std::vector<int>& data) const {
    return std::is_sorted(data.begin(), data.end());
}

Benchmark::TestResult Benchmark::runSingleTest(
    SortingAlgorithm* algo,
    std::vector<int>& data,
    const std::string& dataType,
    size_t dataSize,
    int threadCount
) {
    omp_set_num_threads(threadCount);
    
    auto start = std::chrono::high_resolution_clock::now();
    algo->sort(data);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    TestResult result;
    //result.avgTime = duration.count();
    result.avgTime = static_cast<double>(duration.count()) / 1000.0; // Convert to milliseconds for consistency
    result.dataType = dataType;
    result.dataSize = dataSize;
    result.threadCount = threadCount;
    result.correctness = verifySorting(data);
    
    return result;
}

Benchmark::TestResult Benchmark::calculateStatistics(
    const std::vector<double>& times,
    const std::string& dataType,
    size_t dataSize,
    int threadCount,
    bool correctness
) {
    TestResult result;
    result.dataType = dataType;
    result.dataSize = dataSize;
    result.threadCount = threadCount;
    result.correctness = correctness;
    
    // Calculate average
    result.avgTime = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    
    // Find min and max
    result.minTime = *std::min_element(times.begin(), times.end());
    result.maxTime = *std::max_element(times.begin(), times.end());
    
    // Calculate standard deviation
    double sumSquares = 0;
    for (double time : times) {
        double diff = time - result.avgTime;
        sumSquares += diff * diff;
    }
    result.stdDev = std::sqrt(sumSquares / times.size());
    
    return result;
}

void Benchmark::runBenchmarks(
    const std::vector<size_t>& dataSizes,
    int numTests,
    int maxThreads
) {
    std::vector<std::string> dataTypes = {
        "Random",
        "Nearly Sorted",
        "Reverse Sorted",
        "Few Unique"
    };
    
    std::cout << "\nStarting Benchmark Suite\n";
    std::cout << "----------------------\n";
    
    for (size_t size : dataSizes) {
        std::cout << "\nTesting with data size: " << size << std::endl;
        
        for (const auto& dataType : dataTypes) {
            std::cout << "\nData type: " << dataType << std::endl;
            
            for (const auto& algo : algorithms) {
                for (int threadCount = 1; threadCount <= maxThreads; threadCount *= 2) {
                    if (!algo->isParallel() && threadCount > 1) continue;
                    
                    std::vector<double> times;
                    bool allCorrect = true;
                    
                    for (int test = 0; test < numTests; test++) {
                        // Generate fresh data for each test
                        std::vector<int> data;
                        if (dataType == "Random")
                            data = generateRandomData(size);
                        else if (dataType == "Nearly Sorted")
                            data = generateNearlySortedData(size);
                        else if (dataType == "Reverse Sorted")
                            data = generateReverseSortedData(size);
                        else
                            data = generateFewUniqueData(size);
                        
                        auto result = runSingleTest(algo.get(), data, dataType, size, threadCount);
                        times.push_back(result.avgTime);
                        allCorrect &= result.correctness;
                    }
                    
                    auto stats = calculateStatistics(times, dataType, size, threadCount, allCorrect);
                    
                    // Calculate speedup relative to sequential version if this is parallel
                    if (algo->isParallel() && threadCount > 1) {
                        // Find sequential time for this algorithm/size/datatype combination
                        for (const auto& result : results[algo->getName()]) {
                            if (result.dataSize == size && 
                                result.dataType == dataType && 
                                result.threadCount == 1) {
                                stats.speedup = result.avgTime / stats.avgTime;
                                break;
                            }
                        }
                    } else {
                        stats.speedup = 1.0;
                    }
                    
                    results[algo->getName()].push_back(stats);
                    
                    // Print immediate results
                    std::cout << algo->getName() 
                             << " (Threads: " << threadCount << "): "
                             << stats.avgTime << "ms"
                             << " (Speedup: " << stats.speedup << "x)"
                             << (stats.correctness ? " ✓" : " ✗") << std::endl;
                }
            }
        }
    }
}

void Benchmark::exportResults(const std::string& filename) const {
    std::ofstream file(filename);
    
    // Write CSV header
    file << "Algorithm,Data Size,Data Type,Thread Count,Average Time (ms),"
         << "Min Time (ms),Max Time (ms),Std Dev,Speedup,Correctness\n";
    
    // Write results
    for (const auto& [algoName, results] : results) {
        for (const auto& result : results) {
            file << algoName << ","
                 << result.dataSize << ","
                 << result.dataType << ","
                 << result.threadCount << ","
                 << result.avgTime << ","
                 << result.minTime << ","
                 << result.maxTime << ","
                 << result.stdDev << ","
                 << result.speedup << ","
                 << (result.correctness ? "true" : "false") << "\n";
        }
    }
}

void Benchmark::printResults() const {
    std::cout << "\nBenchmark Results Summary\n";
    std::cout << "------------------------\n";
    
    for (const auto& [algoName, results] : results) {
        std::cout << "\n" << algoName << ":\n";
        
        for (const auto& result : results) {
            std::cout << "Size: " << std::setw(8) << result.dataSize 
                     << " | Type: " << std::setw(13) << result.dataType
                     << " | Threads: " << std::setw(2) << result.threadCount
                     << " | Time: " << std::setw(8) << result.avgTime << "ms"
                     << " | Speedup: " << std::setw(5) << std::fixed << std::setprecision(2) 
                     << result.speedup << "x"
                     << (result.correctness ? " ✓" : " ✗") << "\n";
        }
    }
}

const std::map<std::string, std::vector<Benchmark::TestResult>>& 
Benchmark::getResults() const {
    return results;
}

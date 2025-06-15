#ifndef ALGORITHM_RESULT_H
#define ALGORITHM_RESULT_H

#include <string>
#include <vector>
#include <cstdint>

/**
 * Structure to hold the results of algorithm execution
 * For selection algorithms (QuickSelect, SelectLinear):
 *   - value: the k-th smallest element
 *   - result: empty vector (not used)
 * For sorting algorithms (QuickSort, MergeSort):
 *   - value: 0 (not used)
 *   - result: sorted vector
 */
struct AlgorithmResult {
    std::string algorithm_name = "";
    int value = 0;                    // For selection algorithms
    std::vector<int> result;          // For sorting algorithms
    double execution_time = 0.0;      // in milliseconds
    uint64_t comparisons = 0;         // number of comparisons
    size_t memory_usage = 0;          // in bytes

    // Constructor for selection algorithms
    static AlgorithmResult forSelection(std::string algorithm_name, int val, double time, uint64_t comps, size_t mem) {
        return {algorithm_name, val, {}, time, comps, mem};
    }

    // Constructor for sorting algorithms
    static AlgorithmResult forSorting(std::string algorithm_name, std::vector<int>&& res, double time, uint64_t comps, size_t mem) {
        return {algorithm_name, 0, std::move(res), time, comps, mem};
    }
};

#endif // ALGORITHM_RESULT_H

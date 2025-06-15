#ifndef QUICK_SELECT_H
#define QUICK_SELECT_H

#include <vector>
#include <cstdint>  // for uint64_t
#include <chrono>  // for timing
#include <cstdlib>  // for rand()
#include <algorithm> // for std::swap
#include "../AlgorithmResult.h"

/**
 * @class QuickSelect
 * @brief Implements the QuickSelect algorithm to find the k-th smallest element in an unsorted array.
 */
class QuickSelect {
private:
    /**
     * Partitions the array around a pivot element such that elements smaller than the pivot
     * are on the left and elements greater than the pivot are on the right.
     * 
     * @param data The array to be partitioned
     * @param left The starting index of the partition
     * @param right The ending index of the partition
     * @param[out] comparison_count Counter for element comparisons
     * @return The final position of the pivot element
     */
    int partition(std::vector<int>& data, int left, int right, uint64_t& comparison_count) const {
        // Choose the rightmost element as pivot
        int pivot_value = data[right];
        int smaller_element_index = left - 1;

        for (int current_index = left; current_index < right; ++current_index) {
            // If current element is smaller than or equal to pivot
            comparison_count++;
            if (data[current_index] <= pivot_value) {
                ++smaller_element_index;
                std::swap(data[smaller_element_index], data[current_index]);
            }
        }
        
        // Place the pivot element in its correct position
        std::swap(data[smaller_element_index + 1], data[right]);
        return smaller_element_index + 1;
    }

    /**
     * Selects a random pivot and partitions the array around it.
     * 
     * @param data The array to be partitioned
     * @param left The starting index of the partition
     * @param right The ending index of the partition
     * @param[out] comparison_count Counter for element comparisons
     * @return The final position of the randomly selected pivot
     */
    int randomPartition(std::vector<int>& data, int left, int right, uint64_t& comparison_count) const {
        // Generate a random number between left and right
        int random_index = left + rand() % (right - left + 1);
        
        // Swap the element at random index with the rightmost element
        std::swap(data[random_index], data[right]);
        
        return partition(data, left, right, comparison_count);
    }

    /**
     * Finds the k-th smallest element in the array using Quickselect algorithm.
     * 
     * @param data The input array
     * @param left The starting index of the current partition
     * @param right The ending index of the current partition
     * @param k The position of the element to find (0-based index)
     * @param[out] comparison_count Counter for element comparisons
     * @return The k-th smallest element
     */
    int quickSelect(std::vector<int>& data, int left, int right, int k, uint64_t& comparison_count) const {
        if (left == right) {
            return data[left];
        }

        int pivot_index = randomPartition(data, left, right, comparison_count);
        
        int elements_before_pivot = pivot_index - left + 1;
        comparison_count++;

        if (k == elements_before_pivot - 1) {
            return data[pivot_index];
        } 
        
        if (k < elements_before_pivot - 1) {
            return quickSelect(data, left, pivot_index - 1, k, comparison_count);
        } 
        else {
            return quickSelect(data, pivot_index + 1, right, k - elements_before_pivot, comparison_count);
        }
    }

public:
    /**
     * @brief Finds the k-th smallest element in the array with performance metrics.
     * 
     * @param data The input array (will be modified during execution)
     * @param k The position of the element to find (0-based index)
     * @return AlgorithmResult containing the result and performance metrics
     */
    AlgorithmResult quickSelectWithMetrics(const std::vector<int>& data, int k) const {
        auto start_time = std::chrono::high_resolution_clock::now();
        uint64_t comparisons = 0;
        
        // Make a copy to avoid modifying the original array
        std::vector<int> data_copy = data;
        
        // Calculate initial memory usage (just the copy of the vector)
        size_t memory_used = sizeof(int) * data_copy.capacity();
        
        // Find the k-th smallest element
        int result = quickSelect(data_copy, 0, data_copy.size() - 1, k, comparisons);
        
        auto end_time = std::chrono::high_resolution_clock::now();
        double execution_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        
        return AlgorithmResult::forSelection("QuickSelect", result, execution_time, comparisons, memory_used);
    }
};

#endif // QUICK_SELECT_H

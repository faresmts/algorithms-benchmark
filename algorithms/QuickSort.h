#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include "../AlgorithmResult.h"

class QuickSort {
private:
    // Member variable to count comparisons
    uint64_t comparisons;
    // Random number generator
    std::random_device rd;
    std::mt19937 gen;
    
    // Private helper methods
    int partition(std::vector<int>& arr, int low, int high);
    int randomPartition(std::vector<int>& arr, int low, int high);
    void quickSort(std::vector<int>& arr, int low, int high);
    
public:
    // Constructor
    QuickSort() : comparisons(0), gen(rd()) {}
    
    // Public interface
    AlgorithmResult sortWithMetrics(std::vector<int> arr);
};

// Implementation of the methods
inline int QuickSort::partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[low];
    int i = low, j = high;

    while (true) {
        // Find leftmost element >= pivot
        while (i <= high && arr[i] < pivot) {
            i++;
            comparisons++;
        }
        comparisons++; // for the last comparison that failed

        // Find rightmost element <= pivot
        while (j >= low && arr[j] > pivot) {
            j--;
            comparisons++;
        }
        comparisons++; // for the last comparison that failed

        // If pointers crossed
        if (i >= j) {
            return j;
        }

        // Swap elements and move pointers
        std::swap(arr[i], arr[j]);
        i++;
        j--;
    }
}

inline int QuickSort::randomPartition(std::vector<int>& arr, int low, int high) {
    // Validate input
    if (low < 0 || high < 0 || low >= arr.size() || high >= arr.size() || low > high) {
        throw std::invalid_argument("Invalid partition indices");
    }
    
    if (low == high) {
        return low;
    }
    
    // Generate random index between low and high (inclusive)
    std::uniform_int_distribution<> distrib(low, high);
    int random = distrib(gen);
    
    // Swap with first element
    std::swap(arr[random], arr[low]);
    
    return partition(arr, low, high);
}

inline void QuickSort::quickSort(std::vector<int>& arr, int low, int high) {
    if (low < 0 || high < 0 || low >= arr.size() || high >= arr.size()) {
        throw std::invalid_argument("Invalid sort indices");
    }
    
    if (low < high) {
        try {
            int pi = randomPartition(arr, low, high);
            quickSort(arr, low, pi);
            quickSort(arr, pi + 1, high);
        } catch (const std::exception& e) {
            std::cerr << "Sorting error: " << e.what() << std::endl;
            throw;
        }
    }
}

inline AlgorithmResult QuickSort::sortWithMetrics(std::vector<int> arr) {
    auto start_time = std::chrono::high_resolution_clock::now();
    comparisons = 0;
    
    if (!arr.empty()) {
        quickSort(arr, 0, arr.size() - 1);
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    double execution_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    
    // QuickSort uses O(log n) stack space in the best/average case
    size_t stack_usage = sizeof(int) * (1 + log2(arr.size()));
    
    return AlgorithmResult::forSorting("QuickSort", std::move(arr), execution_time, comparisons, stack_usage);
}

#endif // QUICKSORT_H

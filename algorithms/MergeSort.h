#ifndef MERGESORT_H
#define MERGESORT_H

#include <vector>
#include <chrono>
#include "../AlgorithmResult.h"

class MergeSort {
private:
    // Member variable to count comparisons
    long long comparisons;

    // Private helper methods
    void merge(std::vector<int>& arr, int l, int m, int r);
    void sort(std::vector<int>& arr, int l, int r);

public:
    // Constructor
    MergeSort() : comparisons(0) {}

    // Public interface
    AlgorithmResult sortWithMetrics(std::vector<int> arr);
};

// Implementation of the methods
inline void MergeSort::merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        comparisons++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

inline void MergeSort::sort(std::vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        sort(arr, l, m);
        sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

inline AlgorithmResult MergeSort::sortWithMetrics(std::vector<int> arr) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Reset comparison counter
    comparisons = 0;
    
    // Calculate initial memory usage (input vector)
    size_t initial_memory = sizeof(int) * arr.capacity();
    
    // Execute merge sort
    sort(arr, 0, arr.size() - 1);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    double execution_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    
    // Additional memory used by merge sort is the size of the temporary arrays
    // which is O(n) in the worst case
    size_t additional_memory = sizeof(int) * arr.size();
    
    return AlgorithmResult::forSorting("MergeSort", std::move(arr), execution_time, comparisons, additional_memory);
}

#endif // MERGESORT_H
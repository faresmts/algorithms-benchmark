#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include "AlgorithmResult.h"

using namespace std;
using namespace std::chrono;

// Global counters
static size_t comparison_count = 0;
static size_t additional_memory = 0;  // Track additional memory used during execution


// encontrar a mediana de um vetor
int median (vector<int>& arr){
    // Count the comparison in the sort
    sort(arr.begin(), arr.end(), [](int a, int b) {
        comparison_count++;
        return a < b;
    });
    return arr[arr.size() / 2];
}


int select_linear(vector<int> arr, int k){

    if (arr.size() <= 5) {
        sort(arr.begin(), arr.end(), [](int a, int b) {
            comparison_count++;
            return a < b;
        });
        return arr[k];
    }

    vector<int> medians;

    for (size_t i = 0; i < arr.size(); i += 5) {
        vector<int> group;
        for (size_t j = i; j < i + 5 && j < arr.size(); ++j)
            group.push_back(arr[j]);
        medians.push_back(median(group));
    }

    additional_memory += medians.capacity() * sizeof(int); 

    int med_of_med = select_linear(medians, medians.size() / 2);

    vector<int> left, right, equal;

    for (int val : arr) {
        comparison_count++;
        if (val < med_of_med) {
            left.push_back(val);
        }else{
            comparison_count++;
            if (val > med_of_med) {
                right.push_back(val);
            } else {
                equal.push_back(val);
            }
        }
    }

    additional_memory += (left.capacity() + right.capacity() + equal.capacity()) * sizeof(int);

    if (k < (int)left.size())
        return select_linear(left, k);
    else if (k < (int)(left.size() + equal.size()))
        return med_of_med;
    else
        return select_linear(right, k - left.size() - equal.size());
}

/**
 * Wrapper function for SelectLinear with metrics collection
 * 
 * @param data The input vector
 * @param k The position of the element to find (0-based index)
 * @return AlgorithmResult containing the result and performance metrics
 */
AlgorithmResult selectLinearWithMetrics(const vector<int>& data, int k) {
    auto start_time = high_resolution_clock::now();
    comparison_count = 0;
    
    if (k < 0 || k >= static_cast<int>(data.size())) {
        throw out_of_range("k is out of bounds");
    }
    
    // Make a copy to avoid modifying the original array
    vector<int> data_copy = data;
    
    // Execute the algorithm and measure memory usage
    size_t initial_memory = sizeof(int) * data_copy.capacity();
    int result = select_linear(data_copy, k);
    size_t peak_memory = sizeof(int) * data_copy.capacity();
    
    auto end_time = high_resolution_clock::now();
    double execution_time = duration<double, milli>(end_time - start_time).count();
    
    // Use the tracked additional memory, but ensure it's at least the difference between peak and initial
    size_t calculated_memory = (peak_memory > initial_memory) ? peak_memory - initial_memory : 0;
    size_t total_additional = max(additional_memory, calculated_memory);
    
    return AlgorithmResult::forSelection(result, execution_time, comparison_count, total_additional);
}

#ifndef SELECT_LINEAR_H
#define SELECT_LINEAR_H

#include <vector>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include "../AlgorithmResult.h"

class SelectLinear {
private:
    // Member variables to track metrics
    static inline size_t comparison_count = 0;
    static inline size_t additional_memory = 0;

    // Find median of small array (size <= 5)
    static int median(std::vector<int>& arr) {
        std::sort(arr.begin(), arr.end(), [](int a, int b) {
            SelectLinear::comparison_count++;
            return a < b;
        });
        return arr[arr.size() / 2];
    }

    // Recursive function to find k-th smallest element
    static int select_linear(std::vector<int> arr, int k) {
        if (arr.size() <= 5) {
            std::sort(arr.begin(), arr.end(), [](int a, int b) {
                SelectLinear::comparison_count++;
                return a < b;
            });
            return arr[k];
        }

        std::vector<int> medians;
        for (size_t i = 0; i < arr.size(); i += 5) {
            std::vector<int> group;
            for (size_t j = i; j < i + 5 && j < arr.size(); ++j)
                group.push_back(arr[j]);
            medians.push_back(SelectLinear::median(group));
        }

        SelectLinear::additional_memory += medians.capacity() * sizeof(int); 
        int med_of_med = SelectLinear::select_linear(medians, medians.size() / 2);
        std::vector<int> left, right, equal;

        for (int val : arr) {
            SelectLinear::comparison_count++;
            if (val < med_of_med) {
                left.push_back(val);
            } else if (val > med_of_med) {
                SelectLinear::comparison_count++;
                right.push_back(val);
            } else {
                equal.push_back(val);
            }
        }

        SelectLinear::additional_memory += (left.capacity() + right.capacity() + equal.capacity()) * sizeof(int);

        size_t left_size = left.size();
        size_t equal_size = equal.size();
        
        if (k < static_cast<int>(left_size))
            return SelectLinear::select_linear(left, k);
        else if (k < static_cast<int>(left_size + equal_size))
            return med_of_med;
        else
            return SelectLinear::select_linear(right, k - left_size - equal_size);
    }
    
public:
    // Wrapper function for SelectLinear with metrics collection
    static AlgorithmResult selectLinearWithMetrics(const std::vector<int>& data, int k) {
        auto start_time = std::chrono::high_resolution_clock::now();
        SelectLinear::comparison_count = 0;
        SelectLinear::additional_memory = 0;
        
        if (k < 0 || k >= static_cast<int>(data.size()))
            throw std::out_of_range("k is out of bounds");
        
        std::vector<int> data_copy = data;
        size_t initial_memory = sizeof(int) * data_copy.capacity();
        int result = SelectLinear::select_linear(data_copy, k);
        size_t peak_memory = sizeof(int) * data_copy.capacity() + SelectLinear::additional_memory;
        
        auto end_time = std::chrono::high_resolution_clock::now();
        double execution_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        
        return AlgorithmResult::forSelection(
            "SelectLinear",
            result,
            execution_time,
            SelectLinear::comparison_count,
            peak_memory
        );
    }
};

#endif // SELECT_LINEAR_H
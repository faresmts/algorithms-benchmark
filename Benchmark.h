#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <string>
#include <fstream>
#include "AlgorithmResult.h"
#include "algorithms/MergeSort.h"       
#include "algorithms/QuickSort.h"      
#include "algorithms/SelectLinear.h" 
#include "algorithms/QuickSelect.h"

enum class AlgorithmType {
    SELECTION,  
    SORTING     
};

enum class TestCaseType {
    RANDOM,         
    NEARLY_SORTED,   
    REVERSE_SORTED  
};

class Benchmark {
public:
    struct BenchmarkResult {
        std::string algorithm_name;
        TestCaseType test_case;
        size_t input_size;
        double execution_time_ms; 
        size_t comparisons;        
        size_t memory_usage;        
    };

    static void run_benchmark(size_t size, AlgorithmType type, TestCaseType test_case) {
        std::cout << "Running benchmark for size " << size << " and type " << static_cast<int>(type) << "\n";

        std::vector<int> test_vector;
        if (test_case == TestCaseType::RANDOM) {
            test_vector = generate_random_vector(size);
        } else if (test_case == TestCaseType::NEARLY_SORTED) {
            test_vector = generate_nearly_sorted_vector(size);
        } else if (test_case == TestCaseType::REVERSE_SORTED) {
            test_vector = generate_reverse_sorted_vector(size);
        }

        //print vector
        std::cout << "Test vector: ";
        for (int i = 0; i < 10; i++) {
            std::cout << test_vector[i] << " ";
        }
        std::cout << "\n";

        AlgorithmResult first_result;
        AlgorithmResult second_result;
        
        if (type == AlgorithmType::SELECTION) {
            SelectLinear select_linear;
            std::vector<int> select_linear_vector = test_vector;
            // 7th smallest element (index 6 in 0-based indexing)
            first_result = select_linear.selectLinearWithMetrics(select_linear_vector, 6);
            first_result.algorithm_name = "SelectLinear";

            QuickSelect quick_select;
            std::vector<int> quick_select_vector = test_vector;
            second_result = quick_select.quickSelectWithMetrics(quick_select_vector, 6);
            second_result.algorithm_name = "QuickSelect";
        } else if (type == AlgorithmType::SORTING) {
            MergeSort merge_sorter;
            std::vector<int> merge_sort_vector = test_vector;
            first_result = merge_sorter.sortWithMetrics(merge_sort_vector);
            first_result.algorithm_name = "MergeSort";

            QuickSort quick_sorter;
            std::vector<int> quick_sort_vector = test_vector;
            second_result = quick_sorter.sortWithMetrics(quick_sort_vector);
            second_result.algorithm_name = "QuickSort";
        }

        std::cout << "First result: " << first_result.value << "\n";
        std::cout << "Second result: " << second_result.value << "\n";

        std::vector<BenchmarkResult> results = {
            {first_result.algorithm_name, test_case, size, first_result.execution_time, first_result.comparisons, first_result.memory_usage},
            {second_result.algorithm_name, test_case, size, second_result.execution_time, second_result.comparisons, second_result.memory_usage}
        };

        std::string filename = "";
        if (type == AlgorithmType::SELECTION) {
            filename = "selection_benchmark_results.csv";
        } else if (type == AlgorithmType::SORTING) {
            filename = "sorting_benchmark_results.csv";
        }

        save_results_to_csv(results, filename, type);
    }
    
    static void save_results_to_csv(const std::vector<BenchmarkResult>& results, const std::string& filename, AlgorithmType type) {
        std::ofstream outfile(filename, std::ios::app);
        
        if (outfile.tellp() == 0) {
            if (type == AlgorithmType::SELECTION) {
                outfile << "Test Case,Input Size,Execution Time (ms) Select Linear,Execution Time (ms) QuickSelect,Comparisons Select Linear,Comparisons QuickSelect,Memory Usage (bytes) Select Linear,Memory Usage (bytes) QuickSelect\n";
            } else if (type == AlgorithmType::SORTING) {
                outfile << "Test Case,Input Size,Execution Time (ms) Quick Sort,Execution Time (ms) Merge Sort,Comparisons Quick Sort,Comparisons Merge Sort,Memory Usage (bytes) Quick Sort,Memory Usage (bytes) Merge Sort\n";
            }
        }
        
        auto get_test_case_name = [](TestCaseType test_case) -> std::string {
            switch (test_case) {
                case TestCaseType::RANDOM: return "Random";
                case TestCaseType::NEARLY_SORTED: return "Nearly Sorted";
                case TestCaseType::REVERSE_SORTED: return "Reverse Sorted";
                default: return "Unknown";
            }
        };
        
        outfile << get_test_case_name(results[0].test_case) << ","
               << results[0].input_size << ","
               << results[1].execution_time_ms << ","
               << results[0].execution_time_ms << ","
               << results[1].comparisons << ","
               << results[0].comparisons << ","
               << results[1].memory_usage << ","
               << results[0].memory_usage << "\n";
        
        outfile.close();
        std::cout << "\nResults saved to " << filename << "\n";
    }
    
private:
    static std::vector<int> generate_random_vector(size_t size) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, size * 10);
        
        std::vector<int> vec(size);
        for (auto& elem : vec) {
            elem = distrib(gen);
        }
        return vec;
    }
    
    static std::vector<int> generate_nearly_sorted_vector(size_t size) {
        std::vector<int> vec(size);
        for (size_t i = 0; i < size; ++i) {
            vec[i] = static_cast<int>(i + 1);
        }
        
        std::random_device rd;
        std::mt19937 gen(rd());
        size_t num_shuffles = size / 20;
        
        for (size_t i = 0; i < num_shuffles; ++i) {
            std::uniform_int_distribution<size_t> distrib(0, size - 1);
            size_t idx1 = distrib(gen);
            size_t idx2 = distrib(gen);
            std::swap(vec[idx1], vec[idx2]);
        }
        
        return vec;
    }
    
    static std::vector<int> generate_reverse_sorted_vector(size_t size) {
        std::vector<int> vec(size);
        for (size_t i = 0; i < size; ++i) {
            vec[i] = static_cast<int>(size - i);
        }
        return vec;
    }
};

#endif // BENCHMARK_H

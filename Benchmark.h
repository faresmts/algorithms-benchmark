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

        AlgorithmResult first_result;
        AlgorithmResult second_result;
        
        if (type == AlgorithmType::SELECTION) {

            
        } else if (type == AlgorithmType::SORTING) {
            MergeSort merge_sorter;
            std::vector<int> merge_sort_vector = test_vector;
            first_result = merge_sorter.sortWithMetrics(merge_sort_vector);

            QuickSort quick_sorter;
            std::vector<int> quick_sort_vector = test_vector;
            second_result = quick_sorter.sortWithMetrics(quick_sort_vector);
        }

        //prinf vector result from first_result
        std::cout << "Vector result: ";
        for (int i = 0; i < first_result.result.size(); i++) {
            std::cout << first_result.result[i] << " ";
        }
        std::cout << "\n";
        
        std::cout << "Algorithm: " << static_cast<int>(type) << "\n";
        std::cout << "Test case: " << static_cast<int>(test_case) << "\n";
        std::cout << "Input size: " << size << "\n";
        std::cout << "Execution time: " << first_result.execution_time << " ms\n";
        std::cout << "Comparisons: " << first_result.comparisons << "\n";
        std::cout << "Memory usage: " << first_result.memory_usage << " bytes\n";

        std::cout << "\n QuickSort \n";
        std::cout << "Vector result: ";
        for (int i = 0; i < second_result.result.size(); i++) {
            std::cout << second_result.result[i] << " ";
        }
        std::cout << "\n";

        std::cout << "Algorithm: " << static_cast<int>(type) << "\n";
        std::cout << "Test case: " << static_cast<int>(test_case) << "\n";
        std::cout << "Input size: " << size << "\n";
        std::cout << "Execution time: " << second_result.execution_time << " ms\n";
        std::cout << "Comparisons: " << second_result.comparisons << "\n";
        std::cout << "Memory usage: " << second_result.memory_usage << " bytes\n";
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

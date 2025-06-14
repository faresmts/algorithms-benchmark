#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <string>

// Forward declarations of algorithm functions
void quickSort(std::vector<int>& vetor);
void mergeSort(std::vector<int>& arr, int l, int r);
int quickSelect(std::vector<int>& data, int left, int right, int k, uint64_t& comparison_count);
int select_linear(std::vector<int> arr, int k);

class Benchmark {
public:
    // Enum for different types of test cases
    enum class TestCaseType {
        RANDOM,          // Randomly distributed elements
        NEARLY_SORTED,   // 95% sorted, 5% shuffled
        REVERSE_SORTED   // Sorted in descending order
    };

    // Structure to hold benchmark results
    struct BenchmarkResult {
        std::string algorithm_name;
        TestCaseType test_case;
        size_t input_size;
        double execution_time_ms;  // in milliseconds
        size_t comparisons;        // number of comparisons
        size_t memory_usage;       // in bytes
    };

    // Generate test vectors of different types and sizes
    static std::vector<std::vector<int>> generate_test_vectors(size_t size) {
        std::vector<std::vector<int>> test_vectors;
        
        // 1. Random vector
        test_vectors.push_back(generate_random_vector(size));
        
        // 2. Nearly sorted vector (95% sorted, 5% shuffled)
        test_vectors.push_back(generate_nearly_sorted_vector(size));
        
        // 3. Reverse sorted vector
        test_vectors.push_back(generate_reverse_sorted_vector(size));
        
        return test_vectors;
    }

    // Run benchmarks for all algorithms and test cases
    static std::vector<BenchmarkResult> run_benchmark() {
        std::vector<BenchmarkResult> results;
        std::vector<size_t> sizes = {100000, 500000, 1000000};  // 10^5, 5*10^5, 10^6
        
        // This will be implemented later
        // For now, just return empty results
        return results;
    }

private:
    // Generate a vector with random values
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
    
    // Generate a nearly sorted vector (95% sorted, 5% shuffled)
    static std::vector<int> generate_nearly_sorted_vector(size_t size) {
        // First create a sorted vector
        std::vector<int> vec(size);
        for (size_t i = 0; i < size; ++i) {
            vec[i] = static_cast<int>(i + 1);
        }
        
        // Then shuffle 5% of the elements
        std::random_device rd;
        std::mt19937 gen(rd());
        size_t num_shuffles = size / 20;  // 5% of the size
        
        for (size_t i = 0; i < num_shuffles; ++i) {
            std::uniform_int_distribution<size_t> distrib(0, size - 1);
            size_t idx1 = distrib(gen);
            size_t idx2 = distrib(gen);
            std::swap(vec[idx1], vec[idx2]);
        }
        
        return vec;
    }
    
    // Generate a reverse sorted vector
    static std::vector<int> generate_reverse_sorted_vector(size_t size) {
        std::vector<int> vec(size);
        for (size_t i = 0; i < size; ++i) {
            vec[i] = static_cast<int>(size - i);
        }
        return vec;
    }
};

#endif // BENCHMARK_H

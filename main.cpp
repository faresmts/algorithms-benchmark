#include "Benchmark.h"
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

void run_benchmark_suite(size_t execution_count) {
    std::vector<BenchmarkConfig> configurations = {
        {AlgorithmType::SELECTION, 100000, TestCaseType::NEARLY_SORTED, "SELECTION 100K NEARLY_SORTED"},
        {AlgorithmType::SELECTION, 100000, TestCaseType::RANDOM, "SELECTION 100K RANDOM"},
        {AlgorithmType::SELECTION, 100000, TestCaseType::REVERSE_SORTED, "SELECTION 100K REVERSE_SORTED"},
        {AlgorithmType::SELECTION, 500000, TestCaseType::NEARLY_SORTED, "SELECTION 500K NEARLY_SORTED"},
        {AlgorithmType::SELECTION, 500000, TestCaseType::RANDOM, "SELECTION 500K RANDOM"},
        {AlgorithmType::SELECTION, 500000, TestCaseType::REVERSE_SORTED, "SELECTION 500K REVERSE_SORTED"},
        {AlgorithmType::SELECTION, 1000000, TestCaseType::NEARLY_SORTED, "SELECTION 1M NEARLY_SORTED"},
        {AlgorithmType::SELECTION, 1000000, TestCaseType::RANDOM, "SELECTION 1M RANDOM"},
        {AlgorithmType::SELECTION, 1000000, TestCaseType::REVERSE_SORTED, "SELECTION 1M REVERSE_SORTED"},
        {AlgorithmType::SORTING, 100000, TestCaseType::NEARLY_SORTED, "SORTING 100K NEARLY_SORTED"},
        {AlgorithmType::SORTING, 100000, TestCaseType::RANDOM, "SORTING 100K RANDOM"},
        {AlgorithmType::SORTING, 100000, TestCaseType::REVERSE_SORTED, "SORTING 100K REVERSE_SORTED"},
        {AlgorithmType::SORTING, 500000, TestCaseType::NEARLY_SORTED, "SORTING 500K NEARLY_SORTED"},
        {AlgorithmType::SORTING, 500000, TestCaseType::RANDOM, "SORTING 500K RANDOM"},
        {AlgorithmType::SORTING, 500000, TestCaseType::REVERSE_SORTED, "SORTING 500K REVERSE_SORTED"},
        {AlgorithmType::SORTING, 1000000, TestCaseType::NEARLY_SORTED, "SORTING 1M NEARLY_SORTED"},
        {AlgorithmType::SORTING, 1000000, TestCaseType::RANDOM, "SORTING 1M RANDOM"},
        {AlgorithmType::SORTING, 1000000, TestCaseType::REVERSE_SORTED, "SORTING 1M REVERSE_SORTED"}
    };

    for (const auto& config : configurations) {
        std::cout << "\n=== Starting benchmark: " << config.test_name << " ===\n";
        
        for (size_t i = 0; i < execution_count; i++) {
            try {
                Benchmark::run_benchmark(config);
                
                // Print progress every 10%
                if ((i + 1) % (execution_count / 10) == 0) {
                    int progress = ((i + 1) * 100) / execution_count;
                    std::cout << "[" << config.test_name << "] " << progress << "% completed (" 
                              << (i + 1) << "/" << execution_count << ")\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Error in " << config.test_name << " (iteration " << i + 1 << "): " 
                          << e.what() << "\n";
                return;
            }
        }
        
        std::cout << "=== Completed benchmark: " << config.test_name << " ===\n";
    }
}

int main() {
    const size_t execution_count = 100000;
    
    std::cout << "Starting benchmark suite with " << execution_count << " executions per test case\n";
    std::cout << "Total test cases: 18 (2 algorithms * 3 sizes * 3 test cases)\n";
    std::cout << "Estimated time: This may take a while...\n\n";
    
    run_benchmark_suite(execution_count);
    
    std::cout << "\nAll benchmarks completed successfully!\n";
    return 0;
}

#include "Benchmark.h"
#include <cstddef>
#include <iostream>
#include <cstdlib>

int main() {

    AlgorithmType algorithm_type = AlgorithmType::SELECTION;
    size_t vector_size = 1000000;
    TestCaseType test_case = TestCaseType::NEARLY_SORTED;
    size_t execution_count = 1000;

    for (size_t i = 0; i <= execution_count; i++) {
        try {
            Benchmark::run_benchmark(vector_size, algorithm_type, test_case);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    }
    
    return 0;
}

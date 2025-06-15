#include "Benchmark.h"
#include <iostream>
#include <cstdlib>

int main() {

    AlgorithmType algorithm_type = AlgorithmType::SELECTION;
    size_t vector_size = 10;
    TestCaseType test_case = TestCaseType::RANDOM;
    
    try {
        Benchmark::run_benchmark(vector_size, algorithm_type, test_case);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

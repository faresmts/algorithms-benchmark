#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

std::string getOrdinalSuffix(int num);
#include <cstdint>

struct QuickSelectResult {
    int value;              
    double execution_time;   
    uint64_t comparisons;    
    size_t memory_usage;    
};

/**
 * Partitions the array around a pivot element such that elements smaller than the pivot
 * are on the left and elements greater than the pivot are on the right.
 * 
 * @param data The array to be partitioned
 * @param left The starting index of the partition
 * @param right The ending index of the partition
 * @param[out] comparison_count Counter for element comparisons
 * @return The final position of the pivot element
 */
int partition(std::vector<int>& data, int left, int right, uint64_t& comparison_count) {
    // Choose the rightmost element as pivot
    int pivot_value = data[right];
    int smaller_element_index = left - 1;

    for (int current_index = left; current_index < right; ++current_index) {
        // If current element is smaller than or equal to pivot
        comparison_count++;
        if (data[current_index] <= pivot_value) {
            ++smaller_element_index;
            std::swap(data[smaller_element_index], data[current_index]);
        }
    }
    
    // Place the pivot element in its correct position
    std::swap(data[smaller_element_index + 1], data[right]);
    return smaller_element_index + 1;
}

/**
 * Selects a random pivot and partitions the array around it.
 * 
 * @param data The array to be partitioned
 * @param left The starting index of the partition
 * @param right The ending index of the partition
 * @param[out] comparison_count Counter for element comparisons
 * @return The final position of the randomly selected pivot
 */
int randomPartition(std::vector<int>& data, int left, int right, uint64_t& comparison_count) {
    // Generate a random number between left and right
    int random_index = left + rand() % (right - left + 1);
    
    // Swap the element at random index with the rightmost element
    std::swap(data[random_index], data[right]);
    
    return partition(data, left, right, comparison_count);
}

/**
 * Finds the k-th smallest element in the array using Quickselect algorithm.
 * 
 * @param data The input array
 * @param left The starting index of the current partition
 * @param right The ending index of the current partition
 * @param k The position of the element to find (0-based index)
 * @param[out] comparison_count Counter for element comparisons
 * @return The k-th smallest element
 */
int quickSelect(std::vector<int>& data, int left, int right, int k, uint64_t& comparison_count) {
    if (left == right) {
        return data[left];
    }

    int pivot_index = randomPartition(data, left, right, comparison_count);
    
    int elements_before_pivot = pivot_index - left + 1;
    comparison_count++;

    if (k == elements_before_pivot - 1) {
        return data[pivot_index];
    } 
    
    if (k < elements_before_pivot - 1) {
        return quickSelect(data, left, pivot_index - 1, k, comparison_count);
    } 
    else {
        return quickSelect(data, pivot_index + 1, right, k - elements_before_pivot, comparison_count);
    }
}

/**
 * Wrapper function to find the k-th smallest element in the array with performance metrics.
 * 
 * @param data The input array (will be modified during execution)
 * @param k The position of the element to find (0-based index)
 * @return QuickSelectResult containing the result and performance metrics
 */
QuickSelectResult findKthSmallestWithMetrics(std::vector<int> data, int k) {
    if (k < 0 || k >= static_cast<int>(data.size())) {
        throw std::out_of_range("k is out of bounds");
    }
    
    // Initialize metrics
    QuickSelectResult result;
    result.comparisons = 0;
    result.memory_usage = 0;  // QuickSelect uses O(1) additional space
    
    // Seed the random number generator
    srand(time(0));
    
    // Measure execution time
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Execute QuickSelect
    result.value = quickSelect(data, 0, data.size() - 1, k, result.comparisons);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;
    result.execution_time = duration.count();
    
    // Calculate memory usage (stack space for recursion is the main additional memory usage)
    // Each recursive call uses O(1) stack space, and the maximum depth is O(n) in worst case
    // But in practice, it's O(log n) on average
    result.memory_usage = sizeof(int) * (data.size() + 1);  // Input array size + stack frames
    
    return result;
}

int main() {
    std::vector<int> data = {12, 3, 5, 7, 4, 19, 26};
    int k = 2;  // 0-based index (3rd smallest element)
    
    try {
        QuickSelectResult result = findKthSmallestWithMetrics(data, k);
        
        std::cout << "QuickSelect Results:" << std::endl;
        std::cout << "-------------------" << std::endl;
        std::cout << "Input size: " << data.size() << " elements" << std::endl;
        std::cout << "Finding the " << k + 1 << getOrdinalSuffix(k + 1) << " smallest element (0-based index: " << k << ")" << std::endl;
        std::cout << "Result: " << result.value << std::endl;
        std::cout << "Execution time: " << result.execution_time << " ms" << std::endl;
        std::cout << "Number of comparisons: " << result.comparisons << std::endl;
        std::cout << "Estimated additional memory usage: " << result.memory_usage << " bytes" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}

// Helper function to get ordinal suffix (st, nd, rd, th)
std::string getOrdinalSuffix(int num) {
    if (num >= 11 && num <= 13) {
        return "th";
    }
    switch (num % 10) {
        case 1:  return "st";
        case 2:  return "nd";
        case 3:  return "rd";
        default: return "th";
    }
}

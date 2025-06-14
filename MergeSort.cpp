#include <vector>
#include <chrono>
#include "AlgorithmResult.h"

// Forward declarations
void mergeSort(std::vector<int>& arr, int l, int r);
void merge(std::vector<int>& arr, int l, int m, int r);

// Variável global para contar as comparações
long long comparacoes = 0;

// Função para intercalar (merge) duas metades ordenadas
void merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        comparacoes++;
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

// Função auxiliar para medir o Merge Sort
AlgorithmResult mergeSortWithMetrics(std::vector<int> arr) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Reset global comparison counter
    comparacoes = 0;
    
    // Calculate initial memory usage (input vector)
    size_t initial_memory = sizeof(int) * arr.capacity();
    
    // Execute merge sort
    mergeSort(arr, 0, arr.size() - 1);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    double execution_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    
    // Additional memory used by merge sort is the size of the temporary arrays
    // which is O(n) in the worst case
    size_t additional_memory = sizeof(int) * arr.size();
    
    return AlgorithmResult::forSorting(std::move(arr), execution_time, comparacoes, additional_memory);
}

// Função principal do Merge Sort
void mergeSort(std::vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
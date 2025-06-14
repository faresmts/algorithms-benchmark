#include <iostream>
#include <vector>
#include <chrono>
#include <random>    
#include <algorithm>
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

// int main() {
//     // --- Configuração do Teste Aleatório ---
//     const int TAMANHO_VETOR = 1000000;

//     // Cria o vetor com valores sequenciais
//     std::vector<int> arr(TAMANHO_VETOR);
//     for (int i = 0; i < TAMANHO_VETOR; ++i) {
//         arr[i] = i;
//     }

//     // Embaralha o vetor para torná-lo aleatório
//     std::random_device rd;
//     std::mt19937 g(rd());
//     std::shuffle(arr.begin(), arr.end(), g);

//     try {
//         AlgorithmResult result = mergeSortWithMetrics(arr);
        
//         std::cout << "MergeSort Results:" << std::endl;
//         std::cout << "-----------------" << std::endl;
//         std::cout << "Input size: " << TAMANHO_VETOR << " elements" << std::endl;
//         std::cout << "Execution time: " << result.execution_time << " ms" << std::endl;
//         std::cout << "Number of comparisons: " << result.comparisons << std::endl;
//         std::cout << "Estimated additional memory usage: " << result.memory_usage << " bytes" << std::endl;
        
//         std::cout << "\nSorted array: ";
//         for (int num : result.result) {
//             std::cout << num << " ";
//         }
//         std::cout << std::endl;
        
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
    
//     return 0;
// }
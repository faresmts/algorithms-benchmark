#include <iostream>
#include <vector>
#include <chrono>
#include <random>    
#include <algorithm>

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

// Função principal do Merge Sort
void mergeSort(std::vector<int>& arr, int l, int r) {
    if (l >= r) {
        return;
    }
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int main() {
    // --- Configuração do Teste Aleatório ---
    const int TAMANHO_VETOR = 1000000;

    // Cria o vetor com valores sequenciais
    std::vector<int> arr(TAMANHO_VETOR);
    for (int i = 0; i < TAMANHO_VETOR; ++i) {
        arr[i] = i;
    }

    // Embaralha o vetor para torná-lo aleatório
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr.begin(), arr.end(), g);

    // --- Coleta de Métricas ---
    comparacoes = 0; // Reseta o contador para o teste

    // 1. Tempo de execução
    auto start = std::chrono::high_resolution_clock::now();

    mergeSort(arr, 0, TAMANHO_VETOR - 1);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // 2. Uso de memória adicional
    size_t memoria_adicional = TAMANHO_VETOR * sizeof(int);

    std::cout << "\nVetor apos a ordenacao (previa):  ";

    // --- Apresentação dos Resultados ---
    std::cout << "\n--- Metricas de Desempenho ---" << std::endl;
    std::cout << "Tamanho do Vetor: " << TAMANHO_VETOR << std::endl;
    std::cout << "Tempo de execucao: " << diff.count() << " segundos" << std::endl;
    std::cout << "Contagem de comparacoes: " << comparacoes << std::endl;
    std::cout << "Estimativa de uso de memoria adicional: " << memoria_adicional << " bytes (" << memoria_adicional / 1024.0 << " KB)" << std::endl;

    return 0;
}
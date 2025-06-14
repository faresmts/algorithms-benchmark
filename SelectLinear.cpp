#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// calcula o numero de comparações feita
size_t comparison_count = 0;
// calcula a estimativa de memoria consumida
size_t memory_usage = 0;

// struct para as Métricas
struct Result{
    int element;
    long long tempo_execucao_microseg;
    size_t comparacoes;
    size_t memoria_adicional;
};


// encontrar a mediana de um vetor
int median (vector<int>& arr){
    sort(arr.begin(), arr.end(), [](int a, int b) {
        comparison_count++;
        return a < b;
    });
    return arr[arr.size() / 2];
}


int select_linear(vector<int> arr, int k){

    if (arr.size() <= 5) {
        sort(arr.begin(), arr.end(), [](int a, int b) {
            comparison_count++;
            return a < b;
        });
        return arr[k];
    }

    vector<int> medians;

    for (size_t i = 0; i < arr.size(); i += 5) {
        vector<int> group;
        for (size_t j = i; j < i + 5 && j < arr.size(); ++j)
            group.push_back(arr[j]);
        medians.push_back(median(group));
    }

    memory_usage += medians.size() * sizeof(int); 

    int med_of_med = select_linear(medians, medians.size() / 2);

    vector<int> left, right, equal;

    for (int val : arr) {
        comparison_count++;
        if (val < med_of_med) {
            left.push_back(val);
        }else{
            comparison_count++;
            if (val > med_of_med) {
                right.push_back(val);
            } else {
                equal.push_back(val);
            }
        }
    }

    memory_usage += (left.size() + right.size() + equal.size()) * sizeof(int);

    if (k < (int)left.size())
        return select_linear(left, k);
    else if (k < (int)(left.size() + equal.size()))
        return med_of_med;
    else
        return select_linear(right, k - left.size() - equal.size());
}


// facilita para rodar os testes
Result finalResult(vector<int> vetor, int k){
    comparison_count = 0;
    memory_usage = 0;

    auto begin = high_resolution_clock::now();
    int elemento_k = select_linear(vetor, k);
    auto end = high_resolution_clock::now();

    long long tempo_execucao = duration_cast<microseconds>(end - begin).count();

     return {
        elemento_k,
        tempo_execucao,
        comparison_count,
        memory_usage
    };
}

//teste
int main() {
    vector<int> vetor = {1};
    int k = vetor.size()/2; 

    Result r = finalResult(vetor, k);

    cout << " Elemento na posicao " << (k + 1) << ": " << r.element << "\n";
    cout << " Tempo de execucao: " << r.tempo_execucao_microseg << " microssegundos\n";
    cout << " Comparacoes: " << r.comparacoes << "\n";
    cout << " Memoria adicional estimada: " << r.memoria_adicional << " bytes\n";

    return 0;
}





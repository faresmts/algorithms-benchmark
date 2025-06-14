/*
Escolha um pivô: selecione um elemento da matriz como pivô. A escolha do pivô pode variar 
(por exemplo, primeiro elemento, último elemento, elemento aleatório ou mediana).

Particione o Array usando Hoare's partition scheme: Reorganize o array em torno do pivô. Após o particionamento, 
todos os elementos menores que o pivô estarão à sua esquerda e todos os elementos maiores 
que o pivô estarão à sua direita. O pivô estará então em sua posição correta e obtemos o índice do pivô.



Chamada recursiva: aplique recursivamente o mesmo processo aos dois subarrays particionados (esquerda e direita do pivô).

Caso base: a recursão para quando há apenas um elemento restante na submatriz, pois um único elemento já está classificado.
*/


#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <stdexcept>

using namespace std;

// Global random number generator (seeded once at program start)
static random_device rd;
static mt19937 gen(rd());


// Esta função toma o primeiro elemento como pivô, coloca o elemento pivô em sua posição correta 
// posiciona na matriz classificada e coloca todos os elementos menores (menores que o pivô) 
// à esquerda do pivô e todos os elementos maiores à direita
int partition(vector<int>& vetor, int low, int high)
{
    int pivot = vetor[low];
    int i = low - 1, j = high + 1;

    while (true) {

        // Encontre o elemento mais à esquerda maior ou igual ao pivô
        do {
            i++;
        } while (vetor[i] < pivot);

        // Encontre o elemento mais à direita menor ou igual ao pivô
        do {
            j--;
        } while (vetor[j] > pivot);

        // Se os dois ponteiros se cruzam
        if (i >= j)
            return j;

        swap(vetor[i], vetor[j]);
    }
}

/**
 * Gera um pivô aleatório, troca-o com o primeiro elemento e chama a partição
 * 
 * @param vetor Vetor a ser particionado
 * @param low Índice inicial do subvetor
 * @param high Índice final do subvetor
 * @return Índice do pivô após a partição
 * @throw std::invalid_argument Se os índices forem inválidos
 */
int partition_r(vector<int>& vetor, int low, int high) {
    // Validação de entrada
    if (low < 0 || high < 0 || low >= vetor.size() || high >= vetor.size() || low > high) {
        throw invalid_argument("Índices de partição inválidos");
    }
    
    // Se low == high, não há o que particionar
    if (low == high) {
        return low;
    }
    
    // Gera um índice aleatório entre low e high (inclusive)
    uniform_int_distribution<> distrib(low, high);
    int random = distrib(gen);
    
    // Troca o elemento aleatório com o primeiro elemento
    swap(vetor[random], vetor[low]);
    
    return partition(vetor, low, high);
}


/**
 * Implementação do algoritmo QuickSort com pivô aleatório
 * 
 * @param vetor Vetor a ser ordenado
 * @param low Índice inicial do subvetor
 * @param high Índice final do subvetor
 */
void quickSort(vector<int>& vetor, int low, int high) {
    // Validação de entrada
    if (low < 0 || high < 0 || low >= vetor.size() || high >= vetor.size()) {
        throw invalid_argument("Índices de ordenação inválidos");
    }
    
    if (low < high) {
        try {
            int pi = partition_r(vetor, low, high);
            quickSort(vetor, low, pi);
            quickSort(vetor, pi + 1, high);
        } catch (const exception& e) {
            cerr << "Erro durante a ordenação: " << e.what() << endl;
            throw;
        }
    }
}

/**
 * Função de conveniência para ordenar o vetor inteiro
 * 
 * @param vetor Vetor a ser ordenado
 */
void quickSort(vector<int>& vetor) {
    if (!vetor.empty()) {
        quickSort(vetor, 0, vetor.size() - 1);
    }
}



vector<int> gerarVetorAleatorio(int tamanho, int min, int max) {
    vector<int> vetor;
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<> distrib(min, max);
    
    for (int i = 0; i < tamanho; i++) {
        vetor.push_back(distrib(gen));
    }
    return vetor;
}

vector<int> gerarVetorQuaseOrdenado(int tamanho, int min, int maximo) {
    // Primeiro geramos um vetor ordenado
    
    vector<int> vetor(tamanho);
    
    // Se tamanho for 1, retornamos um vetor com o valor min
    if (tamanho == 1) {
        vetor[0] = min;
        return vetor;
    }
    
    // Calculamos o passo como double para evitar perda de precisão
    double passo = static_cast<double>(maximo - min) / (tamanho - 1);

    for (int i = 0; i < tamanho; i++) {
        vetor[i] = min + static_cast<int>(i * passo);
        if (vetor[i] > maximo) vetor[i] = maximo;
    }

    // Calculamos quantos elementos devem ser embaralhados (5%)
    int num_embaralhar = tamanho * 0.05;
    cout << "Numero de elementos embaralhados: " << num_embaralhar << std::endl;
    if (num_embaralhar < 1) num_embaralhar = 1;  // Pelo menos 1 elemento vai ser embaralhado
    
    // Preparamos o gerador de números aleatórios
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<> dist_posicoes(0, tamanho - 1);  // Para posições
    uniform_int_distribution<> dist_valores(min, maximo);      // Para valores

    // Embaralhamos 5% dos elementos
    for (int i = 0; i < num_embaralhar; i++) {
        int pos = dist_posicoes(gen);    // Gera posição aleatória
        vetor[pos] = dist_valores(gen);  // Atribui novo valor
    }
    return vetor;
}

vector<int> gerarVetorInversamenteOrdenado(int tamanho, int min, int maximo) {
    // Gerando um vetor inversamente ordenado
    
    vector<int> vetor(tamanho);
    
    // Se tamanho for 1, retornamos um vetor com o valor min
    if (tamanho == 1) {
        vetor[0] = min;
        return vetor;
    }
    
    // Calculamos o passo como double para evitar perda de precisão
    double passo = static_cast<double>(maximo - min) / (tamanho - 1);

    for (int i = 0; i < tamanho; i++) {
        vetor[i] = maximo - static_cast<int>(i * passo);
        if (vetor[i] < min) vetor[i] = min;
    }
    return vetor;
}


void imprimirVetor(vector<int>& vetor, int tamanho) {
    cout << "[";
    for (int i = 0; i < tamanho; i++) {
        cout << vetor[i];
        if (i != tamanho - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}


int main()
{
    #ifdef _WIN32
        system("cls"); 
    #else
        system("clear"); 
    #endif

    int tamanho = 20; // Tamanho do Vetor
    int min = 1; // Valor Mínimo dos Elementos do Vetor
    int max = 1000; // // Valor Máximo dos Elementos do Vetor
    

    // Para Teste em Vetor Aleatório
    /*
    vector<int> vetor = gerarVetorAleatorio(tamanho, min, max);

    printf("Vetor Gerado: \n");
    imprimirVetor(vetor, tamanho);

    quickSort(vetor, 0, tamanho - 1);

    printf("Vetor Ordenado: \n");
    imprimirVetor(vetor, tamanho);
    */


    // Para Teste em Vetor Quase Ordenado
    /*
    vector<int> vetor = gerarVetorQuaseOrdenado(tamanho, min, max);

    printf("Vetor Gerado: \n");j
    imprimirVetor(vetor, tamanho);

    quickSort(vetor, 0, tamanho - 1);

    printf("Vetor Ordenado: \n");
    imprimirVetor(vetor, tamanho);
    */


    // Para Teste em Vetor Inversamente Ordenado
    /*
    vector<int> vetor = gerarVetorInversamenteOrdenado(tamanho, min, max);

    printf("Vetor Gerado: \n");
    imprimirVetor(vetor, tamanho);

    quickSort(vetor, 0, tamanho - 1);

    printf("Vetor Ordenado: \n");
    imprimirVetor(vetor, tamanho);
    */

    return 0;
}
#include "selection.h"
#include <stdexcept>
#include <random>
#include <algorithm>

using namespace std;

// ============================================================================
// SELEÇÃO ALEATÓRIA
// ============================================================================

int randomized_select(vector<int>& arr, int i) {
    // TODO: IMPLEMENTAR
    // Dica: chame a versão com limites: randomized_select(arr, 0, arr.size()-1, i)
    
    throw runtime_error("randomized_select não implementado");
}

int randomized_select(vector<int>& arr, int p, int r, int i) {
    // TODO: IMPLEMENTAR
    //
    // Algoritmo:
    // 1. Se p == r, retorna arr[p]
    // 2. q = randomized_partition(arr, p, r)
    // 3. k = q - p + 1  (número de elementos no lado esquerdo, incluindo pivô)
    // 4. Se i == k, retorna arr[q] (pivô é o i-ésimo elemento)
    // 5. Se i < k, busca recursivamente em arr[p..q-1]
    // 6. Caso contrário, busca recursivamente em arr[q+1..r] com i ajustado (i-k)
    
    throw runtime_error("randomized_select com limites não implementado");
}

// ============================================================================
// SELEÇÃO DETERMINÍSTICA
// ============================================================================

int deterministic_select(vector<int>& arr, int i) {
    // TODO: IMPLEMENTAR
    // Dica: chame a versão com limites
    
    throw runtime_error("deterministic_select não implementado");
}

int deterministic_select(vector<int>& arr, int p, int r, int i) {
    // TODO: IMPLEMENTAR
    //
    // Algoritmo "Median of Medians":
    // 1. Se array é pequeno (≤ 5 elementos), ordena e retorna i-ésimo
    // 2. Divide arr[p..r] em grupos de 5
    // 3. Para cada grupo, encontra a mediana (ordena e pega elemento do meio)
    // 4. Recursivamente encontra mediana das medianas
    // 5. Particiona array usando mediana das medianas como pivô
    // 6. Recursivamente busca no lado apropriado
    
    throw runtime_error("deterministic_select com limites não implementado");
}

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

int partition(vector<int>& arr, int p, int r) {
    // TODO: IMPLEMENTAR
    //
    // Algoritmo padrão de partição (Lomuto):
    // 1. Escolhe arr[r] como pivô
    // 2. i = p - 1
    // 3. Para j de p até r-1:
    //    - Se arr[j] <= pivô: incrementa i, troca arr[i] com arr[j]
    // 4. Troca arr[i+1] com arr[r]
    // 5. Retorna i+1
    
    throw runtime_error("partition não implementado");
}

int randomized_partition(vector<int>& arr, int p, int r) {
    // TODO: IMPLEMENTAR
    //
    // Algoritmo:
    // 1. Gera índice aleatório i entre p e r
    // 2. Troca arr[i] com arr[r]
    // 3. Chama partition(arr, p, r)
    
    throw runtime_error("randomized_partition não implementado");
}

int median_of_medians(vector<int>& arr, int p, int r) {
    // TODO: IMPLEMENTAR
    //
    // Algoritmo:
    // 1. n = r - p + 1
    // 2. Se n <= 5, ordena arr[p..r] e retorna mediana
    // 3. Divide em grupos de 5
    // 4. Para cada grupo:
    //    - Ordena o grupo
    //    - Coleta mediana (elemento do meio)
    // 5. Recursivamente encontra mediana das medianas coletadas
    
    throw runtime_error("median_of_medians não implementado");
}

int partition_around_value(vector<int>& arr, int p, int r, int pivot_value) {
    // TODO: IMPLEMENTAR
    //
    // Algoritmo:
    // 1. Encontra índice do pivot_value em arr[p..r]
    // 2. Troca pivot com arr[r]
    // 3. Chama partition(arr, p, r)
    
    throw runtime_error("partition_around_value não implementado");
}

void insertion_sort_range(vector<int>& arr, int p, int r) {
    // TODO: IMPLEMENTAR
    //
    // Insertion sort simples para arr[p..r]
    // (usado para grupos pequenos no median_of_medians)
    
    throw runtime_error("insertion_sort_range não implementado");
}


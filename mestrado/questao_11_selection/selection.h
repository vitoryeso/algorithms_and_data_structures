#ifndef SELECTION_H
#define SELECTION_H

#include <vector>

// ============================================================================
// QUESTÃO 11: Algoritmos de Seleção (Cormen 4ª Ed., Seções 9.2 e 9.3)
// ============================================================================

// ============================================================================
// SELEÇÃO ALEATÓRIA (Seção 9.2)
// ============================================================================

/**
 * Encontra o i-ésimo menor elemento usando seleção aleatória.
 * 
 * Complexidade:
 * - Tempo esperado: O(n)
 * - Pior caso: O(n²) (improvável com pivô aleatório)
 * 
 * Algoritmo:
 * 1. Particiona o array usando pivô aleatório
 * 2. Se i está na posição do pivô, retorna
 * 3. Caso contrário, busca recursivamente no lado apropriado
 * 
 * @param arr Array de entrada (será modificado)
 * @param i Posição desejada (1-indexed: 1 = menor, n = maior)
 * @return O i-ésimo menor elemento
 * 
 * TODO: IMPLEMENTAR
 */
int randomized_select(std::vector<int>& arr, int i);

/**
 * Versão auxiliar com limites.
 * 
 * @param arr Array de entrada
 * @param p Índice inicial (0-indexed)
 * @param r Índice final (0-indexed)
 * @param i Posição desejada relativa ao subarray (1-indexed)
 * @return O i-ésimo menor elemento no subarray arr[p..r]
 * 
 * TODO: IMPLEMENTAR
 */
int randomized_select(std::vector<int>& arr, int p, int r, int i);

// ============================================================================
// SELEÇÃO DETERMINÍSTICA (Seção 9.3) - "Median of Medians"
// ============================================================================

/**
 * Encontra o i-ésimo menor elemento usando seleção determinística.
 * 
 * Complexidade:
 * - Tempo garantido: O(n) no pior caso
 * - Constantes maiores que randomized_select
 * 
 * Algoritmo "Median of Medians":
 * 1. Divide array em grupos de 5 elementos
 * 2. Encontra mediana de cada grupo
 * 3. Recursivamente encontra mediana das medianas
 * 4. Usa essa mediana como pivô
 * 5. Particiona e busca recursivamente
 * 
 * @param arr Array de entrada (será modificado)
 * @param i Posição desejada (1-indexed)
 * @return O i-ésimo menor elemento
 * 
 * TODO: IMPLEMENTAR
 */
int deterministic_select(std::vector<int>& arr, int i);

/**
 * Versão auxiliar com limites.
 * 
 * @param arr Array de entrada
 * @param p Índice inicial
 * @param r Índice final
 * @param i Posição desejada relativa ao subarray
 * @return O i-ésimo menor elemento no subarray arr[p..r]
 * 
 * TODO: IMPLEMENTAR
 */
int deterministic_select(std::vector<int>& arr, int p, int r, int i);

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

/**
 * Particiona array usando último elemento como pivô.
 * 
 * @param arr Array de entrada
 * @param p Índice inicial
 * @param r Índice final
 * @return Índice final do pivô após particionamento
 * 
 * TODO: IMPLEMENTAR
 */
int partition(std::vector<int>& arr, int p, int r);

/**
 * Particiona array usando pivô aleatório.
 * 
 * @param arr Array de entrada
 * @param p Índice inicial
 * @param r Índice final
 * @return Índice final do pivô após particionamento
 * 
 * TODO: IMPLEMENTAR
 */
int randomized_partition(std::vector<int>& arr, int p, int r);

/**
 * Encontra mediana das medianas (usado no deterministic_select).
 * 
 * Algoritmo:
 * 1. Divide arr[p..r] em grupos de 5
 * 2. Ordena cada grupo e encontra mediana
 * 3. Recursivamente encontra mediana das medianas
 * 
 * @param arr Array de entrada
 * @param p Índice inicial
 * @param r Índice final
 * @return A mediana das medianas
 * 
 * TODO: IMPLEMENTAR
 */
int median_of_medians(std::vector<int>& arr, int p, int r);

/**
 * Particiona array usando um valor específico como pivô.
 * 
 * @param arr Array de entrada
 * @param p Índice inicial
 * @param r Índice final
 * @param pivot_value Valor a ser usado como pivô
 * @return Índice final do pivô após particionamento
 * 
 * TODO: IMPLEMENTAR (útil para deterministic_select)
 */
int partition_around_value(std::vector<int>& arr, int p, int r, int pivot_value);

/**
 * Ordena subarray pequeno (usado para grupos de 5 no median_of_medians).
 * 
 * @param arr Array de entrada
 * @param p Índice inicial
 * @param r Índice final
 * 
 * TODO: IMPLEMENTAR (pode usar insertion sort)
 */
void insertion_sort_range(std::vector<int>& arr, int p, int r);

#endif // SELECTION_H


#ifndef MINMAX_H
#define MINMAX_H

#include <vector>
#include <utility>

// ============================================================================
// QUESTÃO 10: Min e Max Simultâneos (Cormen 4ª Ed., Seção 9.1)
// ============================================================================

// Estrutura para retornar min e max junto com número de comparações
struct MinMaxResult {
    int min;
    int max;
    long long comparisons;
    
    MinMaxResult() : min(0), max(0), comparisons(0) {}
    MinMaxResult(int mn, int mx, long long comp) 
        : min(mn), max(mx), comparisons(comp) {}
};

// ============================================================================
// ABORDAGEM NÃO-SIMULTÂNEA (2n-2 comparações)
// ============================================================================

/**
 * Encontra mínimo e máximo separadamente.
 * 
 * Primeiro percorre o array para encontrar o mínimo (n-1 comparações),
 * depois percorre novamente para encontrar o máximo (n-1 comparações).
 * Total: 2n-2 comparações.
 * 
 * @param arr Array de entrada
 * @return MinMaxResult contendo min, max e número de comparações
 * 
 * TODO: IMPLEMENTAR
 */
MinMaxResult find_minmax_separate(const std::vector<int>& arr);

// ============================================================================
// ABORDAGEM SIMULTÂNEA (aproximadamente 3n/2 comparações)
// ============================================================================

/**
 * Encontra mínimo e máximo simultaneamente (Cormen, seção 9.1).
 * 
 * Estratégia:
 * 1. Processa elementos em pares
 * 2. Compara os dois elementos do par entre si
 * 3. Compara o menor com o mínimo atual
 * 4. Compara o maior com o máximo atual
 * 5. Se n é ímpar, processa o último elemento separadamente
 * 
 * Total de comparações: 3⌊n/2⌋ (aproximadamente 3n/2)
 * 
 * @param arr Array de entrada
 * @return MinMaxResult contendo min, max e número de comparações
 * 
 * TODO: IMPLEMENTAR
 */
MinMaxResult find_minmax_simultaneous(const std::vector<int>& arr);

// ============================================================================
// VERSÕES SEM CONTAGEM DE COMPARAÇÕES (para uso em produção)
// ============================================================================

/**
 * Versão simplificada da abordagem separada (retorna apenas min e max).
 * 
 * @param arr Array de entrada
 * @return std::pair<int, int> com (min, max)
 * 
 * TODO: IMPLEMENTAR
 */
std::pair<int, int> minmax_separate(const std::vector<int>& arr);

/**
 * Versão simplificada da abordagem simultânea (retorna apenas min e max).
 * 
 * @param arr Array de entrada
 * @return std::pair<int, int> com (min, max)
 * 
 * TODO: IMPLEMENTAR
 */
std::pair<int, int> minmax_simultaneous(const std::vector<int>& arr);

// =========================================================================
// VARIANTES GROK (para comparação de estilo)
// =========================================================================

// Não-simultâneo em um único loop (dois ifs)
std::pair<int, int> minmax_separate_grok(const std::vector<int>& arr);

// Simultâneo usando std::min/std::max e tratamento inline do último elemento
std::pair<int, int> minmax_simultaneous_grok(const std::vector<int>& arr);


#endif // MINMAX_H


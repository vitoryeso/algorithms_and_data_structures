#ifndef WEIGHTED_MEDIAN_H
#define WEIGHTED_MEDIAN_H

#include <vector>

// ============================================================================
// QUESTÃO 12: Mediana Ponderada e Problema 9-3(e) do Cormen
// ============================================================================

// ============================================================================
// MEDIANA PONDERADA
// ============================================================================

/**
 * Encontra a mediana ponderada de um conjunto de valores.
 * 
 * Definição: A mediana ponderada é o elemento x tal que a soma dos pesos
 * dos elementos menores que x é ≤ 1/2 e a soma dos pesos dos elementos
 * maiores que x é ≤ 1/2.
 * 
 * Complexidade: O(n) usando algoritmo baseado em seleção
 * 
 * @param values Array de valores
 * @param weights Array de pesos (devem somar 1.0)
 * @return O valor da mediana ponderada
 * 
 * Nota: Esta função assume que weights soma 1.0 (ou normaliza internamente)
 * 
 * TODO: IMPLEMENTAR
 */
double weighted_median(std::vector<int>& values, std::vector<double>& weights);

/**
 * Versão que retorna o índice da mediana ponderada.
 * 
 * @param values Array de valores
 * @param weights Array de pesos
 * @return Índice do elemento que é a mediana ponderada
 * 
 * TODO: IMPLEMENTAR
 */
int weighted_median_index(std::vector<int>& values, std::vector<double>& weights);

// ============================================================================
// PROBLEMA 9-3(e): PONTO ÓTIMO EM 1D
// ============================================================================

/**
 * Resolve o Problema 9-3(e) do Cormen:
 * 
 * Dados n pontos em uma linha (coordenadas x₁, x₂, ..., xₙ) e pesos
 * w₁, w₂, ..., wₙ, encontre o ponto p que minimiza a soma das distâncias
 * ponderadas:
 * 
 *     Σ wᵢ |xᵢ - p|
 * 
 * Resultado: A resposta é a mediana ponderada dos pontos!
 * 
 * Justificativa: A mediana ponderada minimiza a soma de distâncias absolutas
 * ponderadas (distância L1 ponderada).
 * 
 * @param points Array de coordenadas dos pontos
 * @param weights Array de pesos (um para cada ponto)
 * @return O ponto ótimo p
 * 
 * TODO: IMPLEMENTAR (pode chamar weighted_median)
 */
double optimal_point_weighted(std::vector<int>& points, std::vector<double>& weights);

/**
 * Calcula o custo total (soma das distâncias ponderadas) para um dado ponto.
 * 
 * Útil para verificar que a solução é ótima.
 * 
 * @param points Array de coordenadas dos pontos
 * @param weights Array de pesos
 * @param p O ponto para calcular o custo
 * @return Custo total: Σ wᵢ |xᵢ - p|
 * 
 * TODO: IMPLEMENTAR
 */
double calculate_weighted_distance_cost(const std::vector<int>& points, 
                                       const std::vector<double>& weights,
                                       double p);

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

/**
 * Normaliza pesos para somarem 1.0.
 * 
 * @param weights Array de pesos (será modificado in-place)
 * 
 * TODO: IMPLEMENTAR
 */
void normalize_weights(std::vector<double>& weights);

/**
 * Verifica se pesos estão válidos (todos não-negativos e somam > 0).
 * 
 * @param weights Array de pesos
 * @return true se válidos, false caso contrário
 * 
 * TODO: IMPLEMENTAR
 */
bool validate_weights(const std::vector<double>& weights);

/**
 * Particiona valores e pesos juntos baseado em um pivô.
 * 
 * Usado no algoritmo de mediana ponderada.
 * 
 * @param values Array de valores
 * @param weights Array de pesos (mesmo tamanho que values)
 * @param p Índice inicial
 * @param r Índice final
 * @return Índice do pivô após particionamento
 * 
 * TODO: IMPLEMENTAR
 */
int partition_weighted(std::vector<int>& values, std::vector<double>& weights, 
                      int p, int r);

#endif // WEIGHTED_MEDIAN_H


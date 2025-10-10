#ifndef SORTED_ARRAY_H
#define SORTED_ARRAY_H

#include <vector>

// ============================================================================
// QUESTÃO 13: Array Ordenado
// ============================================================================

/**
 * Estrutura de dados: Array Ordenado
 * 
 * Mantém elementos sempre ordenados.
 * 
 * Complexidades:
 * - INSERT: O(n) - precisa encontrar posição e deslocar elementos
 * - SEARCH: O(log n) - pode usar busca binária
 * - DELETE: O(n) - precisa encontrar e deslocar elementos
 */
class SortedArray {
private:
    std::vector<int> data;
    
public:
    SortedArray() {}
    
    /**
     * Insere um elemento mantendo array ordenado.
     * 
     * Algoritmo:
     * 1. Encontra posição correta (busca binária ou linear)
     * 2. Insere elemento nessa posição (desloca demais)
     * 
     * Complexidade: O(n)
     * 
     * @param key Valor a inserir
     * 
     * TODO: IMPLEMENTAR
     */
    void insert(int key);
    
    /**
     * Busca um elemento no array.
     * 
     * Usa busca binária (array está ordenado).
     * 
     * Complexidade: O(log n)
     * 
     * @param key Valor a buscar
     * @return true se encontrado, false caso contrário
     * 
     * TODO: IMPLEMENTAR
     */
    bool search(int key) const;
    
    /**
     * Remove primeira ocorrência de um elemento.
     * 
     * Algoritmo:
     * 1. Busca elemento (busca binária)
     * 2. Se encontrado, remove e desloca elementos
     * 
     * Complexidade: O(n)
     * 
     * @param key Valor a remover
     * @return true se removido, false se não encontrado
     * 
     * TODO: IMPLEMENTAR
     */
    bool remove(int key);
    
    /**
     * Retorna número de elementos.
     */
    size_t size() const { return data.size(); }
    
    /**
     * Verifica se está vazio.
     */
    bool empty() const { return data.empty(); }
    
    /**
     * Limpa todos os elementos.
     */
    void clear() { data.clear(); }
    
    /**
     * Retorna cópia dos dados (para testes).
     */
    std::vector<int> get_data() const { return data; }
};

#endif // SORTED_ARRAY_H


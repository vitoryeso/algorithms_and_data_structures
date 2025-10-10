#ifndef HASH_CHAINING_H
#define HASH_CHAINING_H

#include <vector>
#include <list>

// ============================================================================
// QUESTÃO 14: Tabela Hash com Encadeamento
// ============================================================================

/**
 * Tabela Hash com Encadeamento (Separate Chaining)
 * 
 * Trata colisões usando listas encadeadas.
 * Cada posição da tabela é uma lista de elementos.
 * 
 * Complexidades (assumindo hash uniforme):
 * - INSERT: O(1) esperado
 * - SEARCH: O(1 + α) esperado, onde α = n/m (load factor)
 * - DELETE: O(1 + α) esperado
 * 
 * Vantagens:
 * - Simples de implementar
 * - Não há limite para número de elementos
 * - Performance degrada suavemente com α alto
 * 
 * Desvantagens:
 * - Usa memória extra para ponteiros das listas
 * - Pior localidade de cache
 */
class HashTableChaining {
private:
    std::vector<std::list<int>> table;
    size_t num_elements;
    size_t table_size;
    
    /**
     * Função hash (divisão).
     * 
     * h(k) = k mod m
     * 
     * @param key Chave a hashear
     * @return Índice na tabela [0, table_size)
     * 
     * TODO: IMPLEMENTAR
     */
    size_t hash(int key) const;
    
public:
    /**
     * Construtor.
     * 
     * @param size Tamanho da tabela hash
     */
    HashTableChaining(size_t size = 1000);
    
    /**
     * Insere elemento na tabela.
     * 
     * Algoritmo:
     * 1. Calcula hash da chave
     * 2. Adiciona elemento à lista nessa posição
     * 
     * Nota: Esta implementação permite duplicatas.
     * Se quiser evitar, busque antes de inserir.
     * 
     * Complexidade: O(1)
     * 
     * @param key Valor a inserir
     * 
     * TODO: IMPLEMENTAR
     */
    void insert(int key);
    
    /**
     * Busca elemento na tabela.
     * 
     * Algoritmo:
     * 1. Calcula hash da chave
     * 2. Busca na lista nessa posição
     * 
     * Complexidade: O(1 + α)
     * 
     * @param key Valor a buscar
     * @return true se encontrado, false caso contrário
     * 
     * TODO: IMPLEMENTAR
     */
    bool search(int key) const;
    
    /**
     * Remove primeira ocorrência de elemento.
     * 
     * Algoritmo:
     * 1. Calcula hash da chave
     * 2. Busca e remove da lista nessa posição
     * 
     * Complexidade: O(1 + α)
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
    size_t size() const { return num_elements; }
    
    /**
     * Retorna tamanho da tabela.
     */
    size_t capacity() const { return table_size; }
    
    /**
     * Calcula fator de carga (load factor).
     * 
     * α = n / m
     * 
     * @return Load factor
     */
    double load_factor() const;
    
    /**
     * Verifica se está vazia.
     */
    bool empty() const { return num_elements == 0; }
    
    /**
     * Limpa todos os elementos.
     * 
     * TODO: IMPLEMENTAR
     */
    void clear();
    
    /**
     * Retorna número de colisões (para análise).
     * 
     * Conta quantas posições têm mais de um elemento.
     * 
     * TODO: IMPLEMENTAR
     */
    size_t count_collisions() const;
    
    /**
     * Retorna comprimento máximo de uma lista (para análise).
     * 
     * TODO: IMPLEMENTAR
     */
    size_t max_chain_length() const;
};

#endif // HASH_CHAINING_H


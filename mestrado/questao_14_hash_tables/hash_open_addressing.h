#ifndef HASH_OPEN_ADDRESSING_H
#define HASH_OPEN_ADDRESSING_H

#include <vector>
#include <optional>

// ============================================================================
// QUESTÃO 14: Tabela Hash com Endereçamento Aberto
// ============================================================================

/**
 * Tabela Hash com Endereçamento Aberto (Open Addressing)
 * 
 * Trata colisões usando probing (neste caso, linear probing).
 * Todos elementos são armazenados no próprio array.
 * 
 * Esta implementação usa linear probing:
 * h(k, i) = (h(k) + i) mod m
 * 
 * Complexidades (assumindo hash uniforme):
 * - INSERT: O(1/(1-α)) esperado
 * - SEARCH: O(1/(1-α)) esperado
 * - DELETE: Complexo (precisa usar "deleted marker")
 * 
 * Vantagens:
 * - Não usa memória extra para ponteiros
 * - Melhor localidade de cache
 * - Mais eficiente para α baixo
 * 
 * Desvantagens:
 * - Performance degrada rapidamente com α alto
 * - Clustering primário (com linear probing)
 * - Precisa rehashing quando α fica muito alto
 * - DELETE é complicado
 */
class HashTableOpenAddressing {
private:
    enum class Status { EMPTY, OCCUPIED, DELETED };
    
    struct Entry {
        int key;
        Status status;
        
        Entry() : key(0), status(Status::EMPTY) {}
    };
    
    std::vector<Entry> table;
    size_t num_elements;
    size_t table_size;
    
    /**
     * Função hash base (divisão).
     * 
     * @param key Chave a hashear
     * @return Índice base [0, table_size)
     * 
     * TODO: IMPLEMENTAR
     */
    size_t hash(int key) const;
    
    /**
     * Função de probing linear.
     * 
     * h(k, i) = (hash(k) + i) mod table_size
     * 
     * @param key Chave
     * @param i Tentativa de probing (0, 1, 2, ...)
     * @return Índice após probing
     * 
     * TODO: IMPLEMENTAR
     */
    size_t probe(int key, size_t i) const;
    
public:
    /**
     * Construtor.
     * 
     * @param size Tamanho da tabela hash
     */
    HashTableOpenAddressing(size_t size = 1000);
    
    /**
     * Insere elemento na tabela.
     * 
     * Algoritmo:
     * 1. Para i = 0 até table_size:
     *    - idx = probe(key, i)
     *    - Se posição está EMPTY ou DELETED:
     *      - Insere key
     *      - Marca como OCCUPIED
     *      - Retorna
     * 2. Se não encontrou posição, tabela está cheia
     * 
     * Complexidade: O(1/(1-α)) esperado
     * 
     * @param key Valor a inserir
     * @return true se inserido, false se tabela cheia
     * 
     * TODO: IMPLEMENTAR
     */
    bool insert(int key);
    
    /**
     * Busca elemento na tabela.
     * 
     * Algoritmo:
     * 1. Para i = 0 até table_size:
     *    - idx = probe(key, i)
     *    - Se posição está EMPTY: elemento não existe
     *    - Se posição está OCCUPIED e key coincide: encontrou
     *    - Se posição está DELETED: continua buscando
     * 2. Se percorreu toda tabela, não encontrou
     * 
     * Complexidade: O(1/(1-α)) esperado
     * 
     * @param key Valor a buscar
     * @return true se encontrado, false caso contrário
     * 
     * TODO: IMPLEMENTAR
     */
    bool search(int key) const;
    
    /**
     * Remove elemento da tabela.
     * 
     * Algoritmo:
     * 1. Busca elemento (similar a search)
     * 2. Quando encontrar, marca como DELETED (não pode marcar EMPTY!)
     * 3. Decrementa num_elements
     * 
     * Importante: Não pode marcar como EMPTY porque isso quebraria
     * cadeias de probing de outros elementos.
     * 
     * Complexidade: O(1/(1-α)) esperado
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
     * Para endereçamento aberto, α deve ser < 1.
     * Tipicamente, recomenda-se manter α < 0.7 para boa performance.
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
     * Conta número de probes necessários para inserir elementos atuais.
     * 
     * Útil para análise de clustering.
     * 
     * TODO: IMPLEMENTAR
     */
    size_t count_total_probes() const;
    
    /**
     * Retorna comprimento da maior sequência de slots ocupados.
     * 
     * Mede clustering primário.
     * 
     * TODO: IMPLEMENTAR
     */
    size_t max_cluster_size() const;
};

#endif // HASH_OPEN_ADDRESSING_H


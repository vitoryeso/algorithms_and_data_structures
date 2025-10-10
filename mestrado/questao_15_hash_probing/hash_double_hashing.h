#ifndef HASH_DOUBLE_HASHING_H
#define HASH_DOUBLE_HASHING_H

#include <vector>

// ============================================================================
// QUESTÃO 15: Tabela Hash com Double Hashing
// ============================================================================

/**
 * Tabela Hash com Double Hashing
 * 
 * h(k, i) = (h₁(k) + i·h₂(k)) mod m
 * 
 * onde:
 * - h₁(k) = k mod m  (hash primário)
 * - h₂(k) = 1 + (k mod (m-1))  (hash secundário)
 * 
 * Características:
 * - Reduz clustering (primário e secundário)
 * - Distribuição mais uniforme que linear probing
 * - Pequeno overhead computacional (segundo hash)
 * - Melhor performance para α alto
 * 
 * Importante: h₂(k) deve ser relativamente primo com m
 * (garantido se m é primo ou h₂(k) ∈ [1, m-1])
 */
class HashTableDoubleHashing {
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
    mutable size_t total_probes;  // Para análise
    
    /**
     * Função hash primária.
     * h₁(k) = k mod m
     * TODO: IMPLEMENTAR
     */
    size_t hash1(int key) const;
    
    /**
     * Função hash secundária.
     * h₂(k) = 1 + (k mod (m-1))
     * 
     * Importante: Deve retornar valor em [1, m-1] para garantir
     * que percorre toda tabela.
     * 
     * TODO: IMPLEMENTAR
     */
    size_t hash2(int key) const;
    
    /**
     * Função de probing com double hashing.
     * h(k, i) = (h₁(k) + i·h₂(k)) mod m
     * TODO: IMPLEMENTAR
     */
    size_t probe(int key, size_t i) const;
    
public:
    HashTableDoubleHashing(size_t size = 1000);
    
    /**
     * Insere elemento.
     * TODO: IMPLEMENTAR
     */
    bool insert(int key);
    
    /**
     * Busca elemento.
     * TODO: IMPLEMENTAR
     */
    bool search(int key) const;
    
    /**
     * Remove elemento.
     * TODO: IMPLEMENTAR
     */
    bool remove(int key);
    
    size_t size() const { return num_elements; }
    size_t capacity() const { return table_size; }
    double load_factor() const;
    bool empty() const { return num_elements == 0; }
    void clear();
    
    /**
     * Retorna número total de probes realizados (para análise).
     */
    size_t get_total_probes() const { return total_probes; }
    
    /**
     * Reseta contador de probes.
     */
    void reset_probe_count() { total_probes = 0; }
    
    /**
     * Mede clustering (deve ser menor que linear probing).
     * TODO: IMPLEMENTAR
     */
    size_t max_cluster_size() const;
    
    /**
     * Calcula média de probes por operação bem-sucedida.
     * TODO: IMPLEMENTAR
     */
    double average_probes_per_search() const;
};

#endif // HASH_DOUBLE_HASHING_H


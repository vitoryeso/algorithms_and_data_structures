#ifndef HASH_LINEAR_PROBING_H
#define HASH_LINEAR_PROBING_H

#include <vector>

// ============================================================================
// QUESTÃO 15: Tabela Hash com Linear Probing
// ============================================================================

/**
 * Tabela Hash com Linear Probing
 * 
 * h(k, i) = (h₁(k) + i) mod m
 * 
 * Características:
 * - Clustering primário: elementos tendem a se agrupar
 * - Simples de implementar
 * - Boa localidade de cache
 * - Performance degrada com α alto devido a clustering
 */
class HashTableLinearProbing {
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
    
    size_t hash(int key) const;
    size_t probe(int key, size_t i) const;
    
public:
    HashTableLinearProbing(size_t size = 1000);
    
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
     * Mede clustering primário.
     * TODO: IMPLEMENTAR
     */
    size_t max_cluster_size() const;
    
    /**
     * Calcula média de probes por operação bem-sucedida.
     * TODO: IMPLEMENTAR
     */
    double average_probes_per_search() const;
};

#endif // HASH_LINEAR_PROBING_H


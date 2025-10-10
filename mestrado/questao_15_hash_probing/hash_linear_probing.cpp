#include "hash_linear_probing.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

HashTableLinearProbing::HashTableLinearProbing(size_t size)
    : table(size), num_elements(0), table_size(size), total_probes(0) {
}

size_t HashTableLinearProbing::hash(int key) const {
    return (key % table_size + table_size) % table_size;
}

size_t HashTableLinearProbing::probe(int key, size_t i) const {
    return (hash(key) + i) % table_size;
}

bool HashTableLinearProbing::insert(int key) {
    for (size_t i = 0; i < table_size; i++) {
        total_probes++;
        size_t idx = probe(key, i);
        if (table[idx].status == Status::EMPTY || table[idx].status == Status::DELETED) {
            table[idx].key = key;
            table[idx].status = Status::OCCUPIED;
            num_elements++;
            return true;
        }
    }
    return false;
}

bool HashTableLinearProbing::search(int key) const {
    for (size_t i = 0; i < table_size; i++) {
        total_probes++;
        size_t idx = probe(key, i);
        if (table[idx].status == Status::EMPTY) {
            return false;
        }
        if (table[idx].status == Status::OCCUPIED && table[idx].key == key) {
            return true;
        }
    }
    return false;
}

bool HashTableLinearProbing::remove(int key) {
    for (size_t i = 0; i < table_size; i++) {
        size_t idx = probe(key, i);
        if (table[idx].status == Status::EMPTY) {
            return false;
        }
        if (table[idx].status == Status::OCCUPIED && table[idx].key == key) {
            table[idx].status = Status::DELETED;
            num_elements--;
            return true;
        }
    }
    return false;
}

double HashTableLinearProbing::load_factor() const {
    return static_cast<double>(num_elements) / table_size;
}

void HashTableLinearProbing::clear() {
    for (auto& entry : table) {
        entry.status = Status::EMPTY;
    }
    num_elements = 0;
    total_probes = 0;
}

size_t HashTableLinearProbing::max_cluster_size() const {
    size_t max_cluster = 0;
    size_t current_cluster = 0;
    
    for (size_t i = 0; i < table_size; i++) {
        if (table[i].status == Status::OCCUPIED || table[i].status == Status::DELETED) {
            current_cluster++;
            max_cluster = max(max_cluster, current_cluster);
        } else {
            current_cluster = 0;
        }
    }
    
    return max_cluster;
}

double HashTableLinearProbing::average_probes_per_search() const {
    if (num_elements == 0) return 0.0;
    
    size_t total = 0;
    for (size_t idx = 0; idx < table_size; idx++) {
        if (table[idx].status == Status::OCCUPIED) {
            int key = table[idx].key;
            for (size_t i = 0; i < table_size; i++) {
                size_t probe_idx = probe(key, i);
                total++;
                if (probe_idx == idx) {
                    break;
                }
            }
        }
    }
    
    return static_cast<double>(total) / num_elements;
}


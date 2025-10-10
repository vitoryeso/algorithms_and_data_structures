#include "hash_double_hashing.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

HashTableDoubleHashing::HashTableDoubleHashing(size_t size)
    : table(size), num_elements(0), table_size(size), total_probes(0) {
}

size_t HashTableDoubleHashing::hash1(int key) const {
    return (key % table_size + table_size) % table_size;
}

size_t HashTableDoubleHashing::hash2(int key) const {
    // Garante retorno em [1, table_size-1]
    if (table_size <= 1) return 1;
    return 1 + ((key % (table_size - 1) + (table_size - 1)) % (table_size - 1));
}

size_t HashTableDoubleHashing::probe(int key, size_t i) const {
    return (hash1(key) + i * hash2(key)) % table_size;
}

bool HashTableDoubleHashing::insert(int key) {
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

bool HashTableDoubleHashing::search(int key) const {
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

bool HashTableDoubleHashing::remove(int key) {
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

double HashTableDoubleHashing::load_factor() const {
    return static_cast<double>(num_elements) / table_size;
}

void HashTableDoubleHashing::clear() {
    for (auto& entry : table) {
        entry.status = Status::EMPTY;
    }
    num_elements = 0;
    total_probes = 0;
}

size_t HashTableDoubleHashing::max_cluster_size() const {
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

double HashTableDoubleHashing::average_probes_per_search() const {
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

